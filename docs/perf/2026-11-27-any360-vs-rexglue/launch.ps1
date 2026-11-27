# Launches TDU once (or several times) on cores 0-7 under the machine lock; screenshots at given seconds (Any360 GPU only).
# Prints "=== <args> (<log>)" per launch for tools/frame_lows.py.
param(
    [string[]]$Variants,            # each an argument string; launched in turn, -Repeat times (alternating)
    [int]$Seconds = 60,
    [int]$Repeat = 1,
    [int[]]$Shots = @(),            # seconds after start at which to trigger a screenshot
    [string]$ShotDir = '',
    [string]$Tag = 'run'
)
$ErrorActionPreference = 'Stop'
$exe = 'F:\TDU\TDURewrite\out\build\tdu.exe'
$logs = 'F:\TDU\TDURewrite\out\build\logs'
$lock = 'F:\Crackdown\any360-machine.lock'
$user = Join-Path $env:TEMP 'tdu-user'   # a copy of the save folder, never the original

while ($true) {
    try { [IO.File]::Open($lock, 'CreateNew').Close(); break }
    catch { Write-Host "lock held: $(Get-Content $lock -ErrorAction SilentlyContinue)"; Start-Sleep 120 }
}
Set-Content $lock "Test Drive Unlimited (refresh-tdu session) $(Get-Date -Format s)"
try {
    for ($r = 0; $r -lt $Repeat; $r++) {
        foreach ($v in $Variants) {
            $before = @(Get-ChildItem $logs -Filter 'tdu_*.log' | ForEach-Object Name)
            $shot = if ($ShotDir) { Join-Path $ShotDir "$Tag-r$r.png" } else { '' }
            $argline = "--user_data_root=`"$user`" $v"
            if ($shot) { $argline += " --any360_gpu_screenshot=`"$shot`"" }
            $p = Start-Process $exe -ArgumentList $argline -WorkingDirectory (Split-Path $exe) -PassThru
            Start-Sleep -Milliseconds 300
            try { $p.ProcessorAffinity = [IntPtr]0xFF } catch {}
            $t0 = Get-Date
            $taken = @()
            while (((Get-Date) - $t0).TotalSeconds -lt $Seconds -and -not $p.HasExited) {
                $el = ((Get-Date) - $t0).TotalSeconds
                foreach ($s in $Shots) {
                    if ($el -ge $s -and $taken -notcontains $s) {
                        $taken += $s
                        if ($shot) {
                            if (Test-Path $shot) { Move-Item $shot ($shot -replace '\.png$', "-prev-$(Get-Random).png") -Force }
                            New-Item -ItemType File "$shot.go" -Force | Out-Null
                            Start-Sleep 1
                            if (Test-Path $shot) { Move-Item $shot ($shot -replace '\.png$', "-t$s.png") -Force }
                        }
                    }
                }
                Start-Sleep -Milliseconds 250
            }
            $exited = $p.HasExited
            if (-not $exited) { Stop-Process -Id $p.Id -Force }
            Start-Sleep 2
            $log = Get-ChildItem $logs -Filter 'tdu_*.log' | Where-Object { $before -notcontains $_.Name } |
                Sort-Object LastWriteTime | Select-Object -Last 1
            Write-Host "=== $v ($($log.Name))$(if ($exited) { ' EXITED early' })"
        }
    }
} finally {
    Remove-Item $lock -Force
}

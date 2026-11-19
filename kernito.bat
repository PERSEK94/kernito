@echo off
title KERNITO Terminal
powershell -NoProfile -Command "$port = New-Object System.IO.Ports.SerialPort COM4, 9600, None, 8, one; $port.Open(); try { while ($true) { if ($port.BytesToRead -gt 0) { Write-Host -NoNewline $port.ReadExisting() } if ([Console]::KeyAvailable) { $key = [Console]::ReadKey($true); $port.Write($key.KeyChar) } Start-Sleep -Milliseconds 10 } } finally { $port.Close() }"
pause

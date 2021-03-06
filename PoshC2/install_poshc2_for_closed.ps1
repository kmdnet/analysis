# unzip & same folder... (https://github.com/nettitude/PoshC2)
# for closed:)

$installpath = (Get-Location).Path + "\tmp\"
$CurrentPath = (Get-Location).Path + "\"
$poshpath = $installpath + "PowershellC2\"
$pathexists = Test-Path $installpath

if (!$pathexists) {
    New-Item $installpath -Type Directory 
}

$bRet = Test-Path "$($CurrentPath)PoshC2-master"


if ($bRet) {
    $pathexists = Test-Path "$($CurrentPath)PowershellC2"

    if (!$pathexists) {
        Move-Item "$($CurrentPath)PoshC2-master" "$($installpath)PowershellC2" 
    } else {
        Copy-Item -Path "$($CurrentPath)\PoshC2-master\*" -Destination "$($installpath)PowershellC2" -Recurse -Force
    }

    $SourceExe = "C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe"
    $ArgumentsToSourceExe = "-exec bypass -c import-module ${poshpath}C2-Server.ps1; C2-Server -PoshPath $poshpath"
    $DestinationPath = "$($installpath)PowershellC2\Start-C2-Server.lnk"
    $WshShell = New-Object -comObject WScript.Shell
    $Shortcut = $WshShell.CreateShortcut($DestinationPath)
    $Shortcut.TargetPath = $SourceExe
    $Shortcut.Arguments = $ArgumentsToSourceExe
    $Shortcut.Save()

    # add run as administrator 
    $bytes = [System.IO.File]::ReadAllBytes("$($installpath)PowershellC2\Start-C2-Server.lnk")
    $bytes[0x15] = $bytes[0x15] -bor 0x20
    [System.IO.File]::WriteAllBytes("$($installpath)PowershellC2\Start-C2-Server.lnk", $bytes)

    $SourceExe = "C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe"
    $ArgumentsToSourceExe = "-exec bypass -c ${poshpath}C2-Installer.ps1 $installpath"
    $DestinationPath = "$($installpath)PowershellC2\Update-PoshC2.lnk"
    $WshShell = New-Object -comObject WScript.Shell
    $Shortcut = $WshShell.CreateShortcut($DestinationPath)
    $Shortcut.TargetPath = $SourceExe
    $Shortcut.Arguments = $ArgumentsToSourceExe
    $Shortcut.Save()

    # add run as administrator 
    $bytes = [System.IO.File]::ReadAllBytes("$($installpath)PowershellC2\Start-C2-Server.lnk")
    $bytes[0x15] = $bytes[0x15] -bor 0x20
    [System.IO.File]::WriteAllBytes("$($installpath)PowershellC2\Start-C2-Server.lnk", $bytes)

    $SourceExe = "C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe"
    $ArgumentsToSourceExe = "-exec bypass -c import-module ${poshpath}C2-Viewer.ps1; c2-viewer -poshpath ${poshpath}"
    $DestinationPath = "$($installpath)PowershellC2\Start-Team-Viewer.lnk"
    $WshShell = New-Object -comObject WScript.Shell
    $Shortcut = $WshShell.CreateShortcut($DestinationPath)
    $Shortcut.TargetPath = $SourceExe
    $Shortcut.Arguments = $ArgumentsToSourceExe
    $Shortcut.Save()

    Write-Host "[+] Sucessfully installed PoshC2"
    
} else {
    Write-Host "Could not install..."
    Start-Sleep 3
}


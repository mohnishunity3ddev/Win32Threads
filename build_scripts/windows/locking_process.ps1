$arg = $args[0]
if(![bool]$arg) {
    Write-Error "lockproc path needed as argument"
    return
}

$currentDir = (Get-Item $arg).FullName
$FileOrFolderPath = $currentDir

if ((Test-Path -Path $FileOrFolderPath) -eq $false) {
    Write-Warning "File or directory does not exist."       
}
else {
    $LockingProcess = CMD /C "openfiles /query /fo table | find /I ""$FileOrFolderPath"""
    Write-Host $LockingProcess
}
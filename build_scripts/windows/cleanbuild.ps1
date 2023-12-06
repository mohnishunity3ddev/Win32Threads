# import all variables from _variables.ps1
. $PSScriptRoot/_variables.ps1

Write-Alert "Cleaning ..."

# Remove all contents of root bin directory
Remove-Item "$BIN_DIR_PATH\*" -Recurse -Force

# Remove all build folder contents of chapter subfolders.
$Chapters = Get-ChildItem -Path $SRC_DIR_PATH\
foreach($Chapter in $Chapters) {
    $ChapterSubFolders = Get-ChildItem -Path $Chapter
    foreach($ChapterSubFolder in $ChapterSubFolders) {
        $SubFolderBuildPath = "$ChapterSubFolder\$CHAPTER_SUBFOLDERS_BUILD_FOLDER_NAME"
        if(Test-Path -Path $SubFolderBuildPath) {
            Remove-Item "$SubFolderBuildPath\*" -Recurse -Force
        } else {
            Write-Warning "$SubFolderBuildPath does not exist. Skipping..."
        }
    }
}

Write-Success "Cleaning Done!"
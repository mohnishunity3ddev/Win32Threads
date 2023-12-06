. $PSScriptRoot/_variables.ps1

$firstArg = $args[0]
$secondArg = $args[1]
$thirdArg = $args[2]

if(![bool]$firstArg -and ![bool]$secondArg) {
    Write-Error "Atleast one argument required."
    return
}

if($firstArg -eq "scripts") {
    Set-Location $BUILD_SCRIPTS_DIR_PATH
    return
}

if($firstArg -eq "root") {
    Set-Location $PROJECT_ROOT_DIR_PATH
    return
}

if($firstArg -eq "bin") {
    Set-Location $BIN_DIR_PATH
    return
}

if($firstArg -eq "extern") {
    Set-Location $PROJECT_ROOT_DIR_PATH\external
    return
}

$chapterArg = $firstArg
$projectArg = $secondArg

$chapterFound = [bool]0
$projectFound = [bool]0

$Chapters = Get-ChildItem -Path $SRC_DIR_PATH\
foreach($Chapter in $Chapters) 
{    
    $ChapterName = Split-Path -Path $Chapter -Leaf

    if($ChapterName -match $chapterArg) 
    {
        $chapterFound = [bool]1
        # Write-Output "Chapter: $ChapterName matched with ChapterArg: $chapterArg. Path: $Chapter"
        $ChapterSubFolders = Get-ChildItem -Path $Chapter

        foreach($ChapterSubFolder in $ChapterSubFolders) 
        {
            $ProjectName = Split-Path -Path $ChapterSubFolder -Leaf
            if($ProjectName -match $projectArg) 
            {
                $projectFound = [bool]1
                # Write-Output "Project: $ProjectName matched with ProjectArg: $projectArg. Path: $ChapterSubFolder"
                if([bool]$thirdArg) {
                    if(Test-Path -Path $ChapterSubFolder\$thirdArg) {
                        Set-Location $ChapterSubFolder\$thirdArg
                        return
                    }
                }
                Set-Location $ChapterSubFolder
                return
            }
        }
    }
}

if(!$chapterFound) {
    Write-Error "Could not find Chapter [$chapterArg] in the project"
    return
}

if(!$projectFound) {
    Write-Error "Could not find Project [$projectArg] in Chapter [$chapterArg]"
    return
}

# Write-Output "Chapter [$chapterArg] Project [$projectArg] FOUND!"

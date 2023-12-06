. $PSScriptRoot/_variables.ps1

$chapterArg = $args[0]
$projectArg = $args[1]

if(![bool]$chapterArg -or ![bool]$projectArg) {
    Write-Error "Two arguments are required. arg1: Chapter Name; arg2: Project Name"
    return
}

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

                $SubFolderBuildPath = "$ChapterSubFolder\$CHAPTER_SUBFOLDERS_BUILD_FOLDER_NAME"
                if(Test-Path -Path $SubFolderBuildPath)
                {
                    Set-Location $SubFolderBuildPath
                    $exe = Get-ChildItem $SubFolderBuildPath -Recurse -Include *.exe
                    Invoke-Expression "& `"$exe`""
                    return
                } else {
                    Write-Error "No bin folder present in $SubFolderBuildPath"
                }
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

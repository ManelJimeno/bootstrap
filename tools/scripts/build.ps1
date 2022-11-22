#! /usr/bin/pwsh
<#
.SYNOPSIS
    Build the project
.DESCRIPTION
    It builds the project in two phases, in the first, if the out folder does
    not exist, it installs the necessary dependencies from the Conan repositories and
    introduces the appropriate paths in the project's environment configuration file,
    in the second it builds the library and app.
.EXAMPLE
    ./scripts/build.ps1
    Build the project in debug mode
.EXAMPLE
    ./scripts/build.ps1 -r
    Build the project in release mode
.EXAMPLE
    ./scripts/build.ps1 -t
    Build the project in release mode and test
.EXAMPLE
    ./scripts/build.ps1 -i
    Build the project in release mode, test and build the installer
.EXAMPLE
    ./scripts/build.ps1 -h
    Shows the help.
#>

    [CmdletBinding()]
Param(
    [Parameter()]
    [Alias('r')]
    [switch]$release,
    [Parameter()]
    [Alias('i')]
    [switch]$installer,
    [Parameter()]
    [Alias('t')]
    [switch]$doTest,
    [Alias('c')]
    [string]$conanRemote,
    [Alias('d')]
    [switch]$remove_entry,
    [Alias('e')]
    [switch]$loadVS,
    [Parameter()]
    [Alias('h')]
    [switch]$help
)

function show_help
{
    # Display Help
    Write-Output "Build the proyect."
    Write-Output ""
    Write-Output "Syntax: $( $CommandName ) [-r] [-i] [-t] [-h]"
    Write-Output "options:"
    Write-Output "   -r build the release version.`n"
    Write-Output "   -i build the installer.`n"
    Write-Output "   -t execute the test.`n"
    Write-Output "   -e load VS environment.`n"
    Write-Output "   -c add a conan remote.`n"
    Write-Output "   -d used with the above option, removes the entry in the Conan remote entry.`n"
    Write-Output "   -h show this help.`n"
}

if ($help)
{
    show_help
    exit
}

function RunCommand
{
    param(
        [parameter(Mandatory, HelpMessage = "Description")]
        [string]$description,
        [parameter(Mandatory, HelpMessage = "Command")]
        [string]$command,
        [parameter(HelpMessage = "File to store the log")]
        [string]$file
    )
    Write-Host("$description`n") -ForegroundColor White
    Write-Host("$command`n") -ForegroundColor Yellow
    try
    {
        if ($file)
        {
            Invoke-Expression $command | Tee-Object $file
        }
        else
        {
            Invoke-Expression $command
        }
    }
    catch
    {
        Write-Error "Error executing command"
    }

}

Invoke-Expression "conan config set general.revisions_enabled=True"

if ($IsWindows)
{
    conan config set general.use_always_short_paths=True
}
conan config set general.revisions_enabled=True

if ($conanRemote)
{
    if ($remove_entry)
    {
        conan remote remove custom-conan
    }
    conan remote add -i 0 custom-conan $conanRemote False
}

$orginalFolder = $( Get-Location ).Path
$programPath = $( Split-Path $MyInvocation.MyCommand.Path -Parent )
$rootFolder = [System.IO.Path]::GetFullPath($programPath + "/.." * 2).ToString() -replace '[\\]', '/'
$buildType = "Debug"
if ($release)
{
    $buildType = "Release"
}
$installPrefix = "$rootFolder/out/install"
$buildPrefix = "$rootFolder/out/build"
$options = "-DPRE_COMMIT_TOOL=1 "
if ($installer)
{
    $options += "-DBUILD_INSTALLER=ON "
}
$application = $( Join-Path ".\out\build\windows-debug\" "bin\" "application.exe" )

if ($IsLinux)
{
    $requirements = "sudo apt install libgl-dev libx11-xcb-dev libfontenc-dev libxaw7-dev libxcomposite-dev " +
            "libxcursor-dev libxdamage-dev libxfixes-dev libxi-dev libxinerama-dev libxmu-dev " +
            "libxmuu-dev libxpm-dev libxrandr-dev libxres-dev libxss-dev libxtst-dev libxv-dev " +
            "libxvmc-dev libxxf86vm-dev libxcb-render-util0-dev libxcb-xkb-dev libxcb-icccm4-dev " +
            "libxcb-image0-dev libxcb-keysyms1-dev libxcb-randr0-dev libxcb-shape0-dev libxcb-sync-dev " +
            "libxcb-xfixes0-dev libxcb-xinerama0-dev libxcb-dri3-dev libxcb-util-dev"

    Write-Host("This script is running under Linux`n$( $PSVersionTable.OS )`n") -ForegroundColor Green
    $operatingSystem = "linux"
}
elseif ($IsMacOS)
{
    Write-Host("This script is running under macOS`n$( $PSVersionTable.OS )`n") -ForegroundColor Green
    $operatingSystem = "macos"
}
elseif ($IsWindows)
{
    Write-Host("This script is running under windows`n$( $PSVersionTable.OS )`n") -ForegroundColor Green
    $operatingSystem = "windows"
    Invoke-Expression "conan config set general.use_always_short_paths=True"
    $options += "-DCMAKE_CXX_COMPILER=cl "
    if ($loadVS)
    {
        . $programPath\Set-VsEnvironment.ps1
        Set-VsCmd 2019
    }
}

$workFolder = "$operatingSystem-$($buildType.ToLower() )"

if ($requirements)
{
    RunCommand -description "Install requeriments" -command $requirements
}

$command = "cmake -DCMAKE_BUILD_TYPE=$buildType -DCMAKE_INSTALL_PREFIX=$installPrefix/$workFolder $options -S. -B$buildPrefix/$workFolder -G Ninja "

try
{

    if (!(Test-Path $application))
    {
        RunCommand -description "Configure CMake" `
            -command $command `
            -file "./configure_cmake.log"
    }

    RunCommand -description "Building" `
        -command "cmake --build $buildPrefix/$workFolder" `
        -file "./build.log"

    if ($doTest)
    {
        RunCommand -description "Testing" `
            -command "Push-Location $buildPrefix/$workFolder/bin; cmd /c ..\project_env.bat test_settings.exe; Pop-Location"
    }

    if ($installer)
    {
        RunCommand -description "Creating installer" `
            -command "cmake --build $buildPrefix/$workFolder --target package"
    }
}
catch
{
    Set-Location $orginalFolder
    return $?
}

Set-Location $orginalFolder

if ($conanRemote)
{
    conan upload "*" --all -r custom-conan -c
}

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
    [Parameter()]
    [Alias('h')]
    [switch]$help
)

Invoke-Expression "conan config set general.revisions_enabled=True"

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
    if ($file)
    {
        Invoke-Expression $command > $file
    }
    else
    {
        Invoke-Expression $command
    }

}

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
}

if ($requirements)
{
    RunCommand -description "Install requeriments" -command $requirements
}

$programPath = $( Split-Path $MyInvocation.MyCommand.Path -Parent )
$rootFolder = [System.IO.Path]::GetFullPath($programPath + "/.." * 2).ToString() -replace '[\\]', '/'
$buildType = $release ? "Release" : "Debug"
$installPrefix = "$rootFolder/out/install"
$buildPrefix = "$rootFolder/out/build"
$options = $installer ? "-DBUILD_INSTALLER=ON": ""

$workFolder = "$operatingSystem-$($buildType.ToLower() )"

$command = "cmake -DCMAKE_BUILD_TYPE=$buildType -DCMAKE_INSTALL_PREFIX=$installPrefix/$workFolder $options " +
    "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON -S. -B$buildPrefix/$workFolder -G Ninja "

try
{
    RunCommand -description "Configure CMake" `
               -command $command `
               -file "./configure_cmake.log"

    RunCommand -description "Building" `
               -command "cmake --build $buildPrefix/$workFolder" `
               -file "./build.log"

    RunCommand -description "Testing" `
               -command "Push-Location $buildPrefix/$workFolder/bin; ./test_settings; Pop-Location"

    if ($installer)
    {
        RunCommand -description "Creating installer" `
                   -command "cmake --build $buildPrefix/$workFolder --target Package"
    }
}
catch
{
    return $?
}

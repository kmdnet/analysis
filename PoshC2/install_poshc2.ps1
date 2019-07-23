# same Directory unzip folder...(https://github.com/nettitude/PoshC2)
$poshc2_fname = "PoshC2-master"
$install_dir  = "\PowershellC2"

$installpath = Get-Location
$installpath = $installpath.Path + $install_dir + "\"

# exist or not Directory
if(!(Test-Path $installpath)){
    New-Item $installpath -Type Directory
    echo "CreateDirectory"
}

# exist or not PoshC2-master
if(!(Test-Path ($installpath  + $poshc2_fname))){
    Copy-Item $poshc2_fname $installpath
    "copy Poshc2 folder"
}

$bRet = Test-Path ($installpath + "\" + $poshc2_fname)
if($bRet){

}else{
    echo "something fatal..."
}


#echo $installpath 
$env:PYTHONDONTWRITEBYTECODE="1"

if (-not $args.count) {
 $directory = "./tests"
} else {
 $directory = $args[0]
}

."$env:MAYA_LOCATION/bin/mayapy.exe" -B -m nose -xv $directory
[ ! -d "./build_linux" ] && mkdir ./build_linux
pushd ./build_linux
    gcc ../clike.c -o ./clike
popd

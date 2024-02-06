# Final project

## how to build the project

### Short (this works for linux)

We have created a script that builds and runs the project:

```shell
./run
```

Looks like this:

```shell
#!/bin/bash

folder='build'
if [ ! -d "$folder" ]; then
    echo "$folder older folder not found. Creating build folder ..."
    mkdir "$folder"
fi
cd "$folder"
echo -e "\nBuilding cmake ..."
cmake -DCMAKE_BUILD_TYPE=Debug ../
echo -e "\nBuilding make ..."
make
cd 'bin'
echo -e "\nexecuting final_project ..."
./shadow_mapping
cd ../..

```

### Long (this works for linux)

creat the build folder

```shell
mkdir build
```

switch to build folder:

```shell
cd build
```

start cmake:

```shell
cmake ..
```

start make:

```shell
make
```

switch to bin folder:

```shell
cd bin
```

execute the result:

```shell
./shadow_mapping
```

### For Windows users

...

## How to use the switch between shadows in the programm

### Key 1

With the key 1, we can switch between following shadow types:

1. no shadow map
2. shadow map with acne
3. shadow map with bias (removes acne)
4. shadow map with poisson sampling
5. shadow map with stratified poisson sampling

### Key 2

Toggles the dynamic bias on and off. Initially it is set to off.

### Key Space

Toggles the light rotation.

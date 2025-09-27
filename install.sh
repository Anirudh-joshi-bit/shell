#!/bin/bash

# clean the build files
echo make clean
make clean

error=0
isDebug=0

if [ "$#" -eq 0 ]; then

	echo make default
	make

elif [ "$#" -eq 1 ]; then

	if [[ "$1" == "debug" ]]; then

		echo make debug
		isDebug=1
		make debug

	elif [[ "$1" == "default" ]]; then

		echo make default
		make

	else
		echo wrong argument
		error=1
	fi

else
	echo too many argument !
	error=1

fi


if [ $error -eq 0 ]; then

	if [ $isDebug -eq 1 ]; then
		mv build/SHELL_D ~/.local/bin/

	else
		mv build/SHELL ~/.local/bin/
	fi

	grep -qxF 'export PATH=$PATH:$HOME/.local/bin' ~/.zshrc || echo 'export PATH=$PATH:$HOME/.local/bin' >> ~/.zshrc
    grep -qxF 'export PATH=$PATH:$HOME/.local/bin' ~/.bashrc || echo 'export PATH=$PATH:$HOME/.local/bin' >> ~/.bashrc

fi

#!/usr/bin/env bash

BASH_CONFIG="$HOME/.bashrc"
ZSH_CONFIG="$HOME/.zshrc"
FISH_CONFIG="$HOME/.config/config.fish"
LATEST_VERSION="0.4.0"
LOCAL_BIN_DIR="./bin"
CURRENT_SHELL=""
BASH_EXISTS=false
ZSH_EXISTS=false
FISH_EXISTS=false
TARGET=""
INSTALL_DIR_PATH="$HOME/.lily"
INSTALL_LATEST_DIR_PATH="$INSTALL_DIR_PATH/latest"
INSTALL_LATEST_BIN_DIR_PATH="$INSTALL_LATEST_DIR_PATH/bin"
INSTALL_LATEST_LIB_DIR_PATH="$INSTALL_LATEST_DIR_PATH/lib"
INSTALL_LATEST_PM_DIR_PATH="$INSTALL_LATEST_DIR_PATH/pm"
INSTALL_LATEST_PM_PACKAGE_DIR_PATH="$INSTALL_LATEST_PM_DIR_PATH/package"
INSTALL_LATEST_PM_LIB_DIR_PATH="$INSTALL_LATEST_PM_DIR_PATH/lib"

# Check if the $INSTALL_DIR already exists
if [ -d $INSTALL_DIR_PATH ]
then
	echo "Install directory path already exists: $INSTALL_DIR_PATH"
	exit 1
fi

if [ ! -d $LOCAL_BIN_DIR ]
then
	echo "Is expected to build before install Lily"
	exit 1
fi

# Get (OS and Arch) the target
case $(uname -ms) in
	"Darwin x86_64")
		TARGET="darwin-x64"
		;;
	"Darwin arm64")
		TARGET="darwin-arm64"
		;;
	"Linux arm64" | "Linux aarch64")
		TARGET="linux-arm64"
		;;
	"Linux x86_64" | *)
		TARGET="linux-x86-64"
		;;
esac

# Get the current shell
if [[ $SHELL =~ .*bash ]]
then
	CURRENT_SHELL="bash"
elif [[ $SHELL =~ .*zsh ]]
then
	CURRENT_SHELL="zsh"
elif [[ $SHELL =~ .*fish ]]
then
	CURRENT_SHELL="fish"
else
	echo "error: this shell is not yet supported: $SHELL"
	exit 1
fi

if [ -f "/bin/bash" ]
then
	BASH_EXISTS=true
fi

if [ -f "/bin/zsh" ]
then
	ZSH_EXISTS=true
fi

if [ -f "/bin/fish" ]
then
	FISH_EXISTS=true
fi

# Create $HOME/.lily directory
mkdir $INSTALL_DIR_PATH
# Create $HOME/.lily/latest directory
mkdir $INSTALL_LATEST_DIR_PATH
# Create $HOME/.lily/latest/bin directory
mkdir $INSTALL_LATEST_BIN_DIR_PATH
# Create $HOME/.lily/latest/lib directory
mkdir $INSTALL_LATEST_LIB_DIR_PATH
# Create $HOME/.lily/latest/pm directory
mkdir $INSTALL_LATEST_PM_DIR_PATH
# Create $HOME/.lily/latest/pm/package directory
mkdir $INSTALL_LATEST_PM_PACKAGE_DIR_PATH
# Create $HOME/.lily/latest/pm/lib directory
mkdir $INSTALL_LATEST_PM_LIB_DIR_PATH
# Create $HOME/.lily/latest/.version and write the latest version
echo "$LATEST_VERSION" > $INSTALL_LATEST_DIR_PATH/.version

# Copy `lily` and `lilyc` to $INSTALL_LATEST_BIN_DIR_PATH
cp $LOCAL_BIN_DIR/lily $INSTALL_LATEST_BIN_DIR_PATH
cp $LOCAL_BIN_DIR/lilyc $INSTALL_LATEST_BIN_DIR_PATH

# Copy `liblily_sys.so/dylib` and `liblily_builtin.so/dylib` to $INSTALL_LATEST_LIB_DIR_PATH.
if [[ $TARGET =~ darwin.* ]]
then
	cp $LOCAL_BIN_DIR/liblily_sys.dylib $INSTALL_LATEST_LIB_DIR_PATH
	cp $LOCAL_BIN_DIR/liblily_builtin.dylib $INSTALL_LATEST_LIB_DIR_PATH
elif [[ $TARGET =~ linux.* ]]
then
	cp $LOCAL_BIN_DIR/liblily_sys.so $INSTALL_LATEST_LIB_DIR_PATH
	cp $LOCAL_BIN_DIR/liblily_builtin.so $INSTALL_LATEST_LIB_DIR_PATH
else
	echo "unreachable: This target is not expected"
	exit 1
fi

# Add $HOME/.lily/latest/bin to $PATH on bash
if [ $BASH_EXISTS ]
then
	echo "export PATH=\$PATH:$INSTALL_LATEST_BIN_DIR_PATH" >> $BASH_CONFIG
	source $BASH_CONFIG
fi

# Add $HOME/.lily/latest/bin to $PATH on zsh
if [ $ZSH_EXISTS ]
then
	echo "export PATH=\$PATH:$INSTALL_LATEST_BIN_DIR_PATH" >> $ZSH_CONFIG
	source $ZSH_CONFIG
fi

# Add $HOME/.lily/latest/bin to $PATH on fish
if [ $FISH_EXISTS ]
then
	echo "set -gx PATH \"$INSTALL_LATEST_BIN_DIR_PATH\" \$PATH" >> $FISH_CONFIG
	source $FISH_CONFIG
fi

# Test the command
lily -v > /dev/null

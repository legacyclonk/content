#!/usr/bin/bash
# example usage: OBJVERSION=4.9.10.8 C4GROUP=~/clonk/c4group ./make_group_updates.sh

set -e

if [ -z "$C4GROUP" ]; then
	echo "Please define the environment variable C4GROUP to point to your c4group binary" >&2
	exit 1
fi

SCRIPT_DIR="$(realpath $(dirname $0))"
if [ -z "$OBJVERSION" ]; then
	OBJVERSION=$(cat "$SCRIPT_DIR/../Version.txt")
	if [ -z "$OBJVERSION" ]; then
		echo "Please define the environment variable OBJVERSION and tell what version the new definitions are (e.g. 4.9.10.8)" >&2
		exit 1
	fi
fi

TARGET_DIR="$(realpath .)"/"lc_${OBJVERSION//./}.c4u"
SRC_DIR="$SCRIPT_DIR/../packed/"

TMP=$(mktemp -d 2>/dev/null || mktemp -d -t 'make_group_updates') # https://unix.stackexchange.com/a/84980
function cleanup {
	rm -rf "$TMP"
}
trap cleanup EXIT

cd "$TMP"
rm -rf "$TARGET_DIR"
mkdir -p "$TARGET_DIR"

while read -r part || [[ -n "$part" ]]; do
	echo "Downloading $part..."
	curl -\# -L -o "$TMP/old.zip" $part
	unzip "$TMP/old.zip"
	for group in "$SRC_DIR/"*.c4?; do
		groupname=$(basename "$group")
		update="$TARGET_DIR/$groupname.c4u"

		if [ -f "$groupname" ]; then
			echo "	Generating $groupname.c4u..."
			"$C4GROUP" "$update" -g "$groupname" "$group" $OBJVERSION
		fi
	done
	rm *
done < "$SCRIPT_DIR/parts.txt"

echo "Packing $TARGET_DIR..."
"$C4GROUP" "$TARGET_DIR" -p

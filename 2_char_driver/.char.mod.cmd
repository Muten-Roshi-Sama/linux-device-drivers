savedcmd_char.mod := printf '%s\n'   char.o | awk '!x[$$0]++ { print("./"$$0) }' > char.mod

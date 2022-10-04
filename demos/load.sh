if [[ -e "$1" ]]; then
  pyocd flash  ./"$1"/build/flexspi_nor_debug/*.elf --target mimxrt1010
else
  echo "Error: Supply directory containing demo to load"
fi

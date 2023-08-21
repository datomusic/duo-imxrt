if [[ -e "$1" ]]; then
  pyocd flash  ./"$1"/build/flexspi_nor_release/*.elf --target mimxrt1010
else
  echo "Error: Supply directory containing application to load"
fi

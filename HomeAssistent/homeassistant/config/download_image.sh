#!/bin/bash

# Funkcija za preuzimanje sadržaja i provjeru odgovora
download_and_check_content() {
  local url=$1
  local output=$2
  local state_sensor=$3

  # Provjerite je li direktorij za preuzimanje dostupan
  download_dir=$(dirname $output)
  if [ ! -d "$download_dir" ];then
    echo "Direktorij $download_dir ne postoji. Kreiram direktorij."
    mkdir -p $download_dir
  fi

  # Preuzmite sadržaj
  echo "Preuzimam sadržaj sa $url..."
  wget -q -O $output $url
  echo "Sadržaj preuzet u $output"

  # Provjerite vrstu sadržaja
  if [ -f "$output" ];then
    file_type=$(file --mime-type -b $output)
    echo "Tip sadržaja: $file_type"

    # Provjerite je li preuzeti sadržaj slika ili tekst
    if [[ $file_type == image/* ]];then
      echo "Sadržaj je slika. Šaljem 'PERSON' poruku."
      mosquitto_pub -h 127.0.0.1 -t "home/sensor/$state_sensor" -m "PERSON"
    else
      echo "Sadržaj nije slika. Šaljem 'NO_PERSON' poruku."
      mosquitto_pub -h 127.0.0.1 -t "home/sensor/$state_sensor" -m "NO_PERSON"
    fi
  else
    echo "Sadržaj nije pronađen. Šaljem 'NO_PERSON' poruku."
    mosquitto_pub -h 127.0.0.1 -t "home/sensor/$state_sensor" -m "NO_PERSON"
  fi
}

# Provjera argumenata
if [ "$1" == "front" ];then
  echo "Provjera prednje kamere..."
  download_and_check_content "http://192.168.223.95:5003/get_image" "/config/www/front_image.jpg" "front_door_image_state"
elif [ "$1" == "back" ];then
  echo "Provjera stražnje kamere..."
  download_and_check_content "http://192.168.223.95:5003/get_image" "/config/www/back_image.jpg" "back_door_image_state"
else
  echo "Pogrešan argument. Koristite 'front' ili 'back'."
fi

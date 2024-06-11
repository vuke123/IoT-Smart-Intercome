#!/bin/bash

download_and_check_image() {
  local url=$1
  local output=$2
  local state_sensor=$3

  # Preuzmite sadržaj
  /usr/bin/wget -q -O $output $url

  # Provjerite je li preuzimanje uspješno
  if [ ! -f "$output" ]; then
    /usr/bin/mosquitto_pub -h 192.168.223.218 -t "home/sensor/$state_sensor" -m "PREUZIMANJE NEUSPJEŠNO"
    return 1
  fi

  # Provjerite sadrži li datoteka tekst "No person detected."
  if /bin/grep -q "No person detected." $output; then
    /usr/bin/mosquitto_pub -h 192.168.223.218 -t "home/sensor/$state_sensor" -m "NO_PERSON"
  else
    # Ako nije tekst, pretpostavite da je slika
    file_type=$(file --mime-type -b $output)
    if [[ -z "$file_type" ]]; then
      /usr/bin/mosquitto_pub -h 192.168.223.218 -t "home/sensor/$state_sensor" -m "PERSON"
    else
      /usr/bin/mosquitto_pub -h 192.168.223.218 -t "home/sensor/$state_sensor" -m "$file_type"
    fi
  fi
}

if [ "$1" == "front" ]; then
  download_and_check_image "http://192.168.223.95:5003/front-door/get_image" "/config/www/front_image.jpg" "front_door_image_state"
elif [ "$1" == "back" ]; then
  download_and_check_image "http://192.168.223.95:5003/back-door/get_image" "/config/www/back_image.jpg" "back_door_image_state"
else
  echo "Pogrešan argument. Koristite 'front' ili 'back'."
fi

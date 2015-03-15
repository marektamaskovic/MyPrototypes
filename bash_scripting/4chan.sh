#!/bin/bash
(
	cd /home/*/4chan || mkdir /home/*/4chan || exit 3

	chanFolder="4chan"

	echo Paste 4chan thread link\:
	read link
					  
	#if [ "$link" = "https://boards.4chan.org/*/thread/[0-9]{1,}/.*" ]
	if [ "$( echo $link | grep -E '.*4chan.org.*' )" ] 
		then
			imglinks=$(mktemp)

			echo "Link is correct"
			
			folder=$(echo $link | grep -E -o '[0-9]{2,}')
			cd "$folder" || mkdir "$folder"
			wget -O - $link | grep -E -o "i\.4cdn\.org\/[a-z]{0,10}\/[0-9]*[^s]\.(jpg|png|gif|webm)" > $imglinks|| echo 42 is a LIE\! | exit 4

			wget -P $folder/ $(sed -n 2~2p $imglinks) || exit 2

			rm "$imglinks"

			exit 0

		else
			exit 1
	fi	
)

if [ $? = 0 ]; then
	echo Download sucesfull in $(pwd);
elif [ $? = 1 ]; then
	echo "Link isn't correct";
elif [$? = 2 ]; then
	echo "Error while downloading"
elif [$? = 3 ]; then
	echo "Error while openin/creating 4chan folder"
elif [$? = 4 ]; then
	echo "Error while procesing image links"
fi
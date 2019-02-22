#include <string.h>
#include <stdio.h>

#include <odid.h>
#include "base64.h"

int odid_wifi_build_ssid(enum odid_message_type type,
			 struct odid_drone_information *drone,
			 char *ssid)
{
	unsigned char odid_message[ODID_MESSAGE_LENGTH];
	char ssid_b64_padding[40];

	if (odid_message_build(ODID_MESSAGE_BASIC_ID, drone, odid_message) < 0)
		return -1;

	/* ODID identifier */
	ssid[0] = '0';
	ssid[1] = 'D';
	/*
	 * TODO: spec says utf-8 encoding, but I'm not sure if that makes sense.
	 * UTF-8 would encode input bytes < 128 to 1 byte, but input > 128 byte
	 * to 2 bytes, resulting in a string more than 32 bytes ...
	 *
	 * just base64 encoder for now ... 22 bytes encodes to 32 bytes of base64
	 * output. The last two bytes are padding (==), let's just skip those to
	 * squeeze it into the 30 byte we have.
	 */
	if (base64_encode(ssid_b64_padding, sizeof(ssid_b64_padding), odid_message, 22) < 0) {
		fprintf(stderr, "base64 encoder failed ...\n");
		return -1;
	}

	memset(&ssid[2], 0, 32 - 2);
	memcpy(&ssid[2], ssid_b64_padding, 30);

	return 0;

}

int odid_wifi_build_vendor_ie(struct odid_drone_information *drone,
			      void *ie)
{
	/* TODO */
	return -1;
}

int odid_wifi_build_nan_action_frame(struct odid_drone_information *drone,
				     void *nan_action_frame)
{
	/* TODO create pack */
	/* TODO write action frame */
	return -1;
}

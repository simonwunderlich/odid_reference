#ifndef ODID_H
#define ODID_H

#include <stdint.h>
#define __packed __attribute((packed))   /* linux kernel compat */

#define PROTOCOL_VERSION_HEADER		0
#define PROTOCOL_VERSION_SPEC		"0.62"

/* list definitions */
enum odid_message_type {
	ODID_MESSAGE_BASIC_ID = 0,
	ODID_MESSAGE_LOCATION = 1,
	ODID_MESSAGE_AUTHENTICATION = 2,
	ODID_MESSAGE_SELF_ID = 3,
	ODID_MESSAGE_OPERATOR = 4,
};

enum odid_id_type {
	ODID_ID_TYPE_NONE = 0,
	ODID_ID_TYPE_SERIAL_NUMBER = 1,
	ODID_ID_TYPE_CAA_ASSIGNED_ID = 2,
	ODID_ID_TYPE_UTM_ASSIGNED_ID = 2,
};

enum odid_drone_type {
	ODID_DRONE_TYPE_NONE = 0,
	ODID_DRONE_TYPE_FIXED_WING_POWERED = 1,
	ODID_DRONE_TYPE_ROTORCRAFT_MULTIROTOR = 2,
	ODID_DRONE_TYPE_LTA_POWERED = 3,	/* Lighter than air */
	ODID_DRONE_TYPE_LTA_UNPOWERED = 4,	/* Balloon */
	ODID_DRONE_TYPE_VTOL = 5,		/* Fixed wing aircraft that can take off vertically) */
	ODID_DRONE_TYPE_FREE_FALL = 6,	/* Parachute */
	ODID_DRONE_TYPE_ROCKET = 7,
	ODID_DRONE_TYPE_GLIDER = 8,
	ODID_DRONE_TYPE_OTHER = 9,
	/* 10 to 15 reserved */
};

enum odid_status {
	ODID_STATUS_UNDECLARED = 0,
	ODID_STATUS_GROUND = 1,
	ODID_STATUS_AIRBORNE = 2,	/* manual control */
	ODID_STATUS_RETURNING_HOME = 3,
	ODID_STATUS_AUTOMATED_MISSION = 4,
	ODID_STATUS_EMERGENCY = 5,
};

/* TODO 
enum odid_horizontal_accuracy {
};

enum odid_vertical_accuracy {
};

enum odid_horizontal_velocity {
};
*/

/* message types */
struct odid_message_header {
	uint8_t message_type:4, protocol_version:4;

} __packed;


struct odid_message_basic_id {
	struct odid_message_header header;
	uint8_t drone_type:4;
	uint8_t id_type:4;
	char drone_id[20];
} __packed;

struct odid_message_location {
	struct odid_message_header header;
	uint8_t flags:4;
	uint8_t status:4;
	uint8_t speed_ns; /* north/south */
	uint8_t speed_ew; /* east/west */
	/* TODO: mark as little endian */
	int32_t vertical_speed;	
	int32_t latitude;
	int32_t longitude;
	int16_t altitude;
	int16_t geodedic_altitude;
	int16_t height_above_takeoff;
	uint8_t horizontal_accuracy:4;
	uint8_t vertical_accuracy:4;
        uint8_t	horizontal_velocity_accuracy:4;
	uint8_t reserved:4;
	uint8_t data_age;	/* in 10ms */
} __packed;

#define ODID_MESSAGE_LENGTH	22

/* TODO: add some asserts here for the message size */

/* general information */
struct odid_drone_information {
	/* Basic ID */
	uint8_t id_type, drone_type;
	char drone_id[20];
	/* location */
	int32_t speed_ns, speed_ew, speed_vertical;
	float latitude, longitude;
	int32_t altitude_msl, altitude_,geo, altitude_agl;
	int8_t HAccuracy, VAccuracy;
	/* TODO: more */
};


/**
 * odid_message_build - builds a message for a specific type
 * @type: message type to build
 * @drone: general drone information
 * @odid_message: buffer which will be filled with message, 22 bytes
 *
 * Returns 0 on success, < 0 on failure. @buf only contains a valid message
 * if the return code is 0
 */
int odid_message_build(enum odid_message_type type,
		       struct odid_drone_information *drone,
		       void *odid_message);

/**
 * odid_wifi_build_ssid - builds a message and encode it as SSID
 * @type: message type to build
 * @drone: general drone information
 * @ssid: buffer which will be filled with the SSID, 32 bytes
 *
 * Returns 0 on success, < 0 on failure. @buf only contains a valid message
 * if the return code is 0
 *
 * TODO: probably obsolete, will be removed.
 */
int odid_wifi_build_ssid(enum odid_message_type type,
			 struct odid_drone_information *drone,
			 char *ssid);

/* odid_wifi_build_vendor_ie - puts current drone information into a vendor IE
 * @drone: general drone status information
 * @ie: vendor information element (needs to be allocated by the caller, 256 byte)
 *
 * Returns 0 on success, and ie will be filled.
 *
 * TODO: probably obsolete, to be removed.
 */
int odid_wifi_build_vendor_ie(struct odid_drone_information *drone,
			      void *ie);

/* odid_wifi_build_nan_action_frame - puts current drone information into an NAN action frame
 * @drone: general drone status information
 * @ie: vendor information element (needs to be allocated by the caller, ?? byte)
 *
 * Returns 0 on success, and ie will be filled.
 */
int odid_wifi_build_nan_action_frame(struct odid_drone_information *drone,
				     void *nan_action_frame);

#endif /* ODID_H */

#include <string.h>

#include <odid.h>

/**
 * odid_message_build_message_id - builds a message for basic id
 * @drone: general drone information
 * @odid_message: buffer which will be filled with message, at least 22 bytes
 *
 * Returns 0 on success, < 0 on failure. @buf only contains a valid message
 * if the return code is 0
 */
static int odid_message_build_basic_id(struct odid_drone_information *drone,
				       void *odid_message)
{
	struct odid_message_basic_id *msg = odid_message;

	msg->id_type = drone->id_type;
	msg->drone_type = drone->drone_type;
	memset(msg->drone_id, 0, sizeof(msg->drone_id));
	strncpy(msg->drone_id, drone->drone_id, sizeof(msg->drone_id));
	return 0;
}

int odid_message_build(enum odid_message_type type,
		       struct odid_drone_information *drone,
		       void *odid_message)
{
	struct odid_message_header *header = odid_message;

	header->message_type = 0;
	header->protocol_version = 0;

	switch (type) {
	case ODID_MESSAGE_BASIC_ID:
		return odid_message_build_basic_id(drone, odid_message);
	default:
		/* unsupported */
		return -1;
	}

	return 0;
}

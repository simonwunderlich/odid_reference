#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <gps.h>

#include <odid.h>

struct global {
	char server[1024];
	char port[16];
};

void usage(char *name)
{
	/* TODO */
	fprintf(stderr,"%s\n", name);
}

int read_arguments(int argc, char *argv[], struct odid_drone_information *drone, struct global *global)
{
	/* TODO: use getopt */

	strncpy(drone->drone_id, "12345678901234567890", sizeof(drone->drone_id));
	drone->id_type = ODID_ID_TYPE_SERIAL_NUMBER;
	drone->drone_type = ODID_DRONE_TYPE_LTA_UNPOWERED; /* balloon */

	strncpy(global->server, "127.0.0.1", sizeof(global->server));
	strncpy(global->port, (char *)DEFAULT_GPSD_PORT, sizeof(global->port));

	return 0;
}

/**
 * drone_adopt_gps_data - adopt GPS data into the drone status info
 * @gpsdata: gps data from gpsd
 * @drone: general drone status information
 */
static void drone_adopt_gps_data(struct odid_drone_information *drone,
				 struct gps_data_t *gpsdata)
{
	printf("GPS: mode %d, lat %f, lon %f\n", gpsdata->fix.mode, gpsdata->fix.latitude, gpsdata->fix.longitude);
	drone->latitude = gpsdata->fix.latitude;
	drone->longitude = gpsdata->fix.longitude;

	/* TODO: read or compute more values like speed, altitude, ... */
}

/**
 * drone_send_data - send information about the drone out
 * @drone: general drone status information
 */
static void drone_send_data(struct odid_drone_information *drone)
{
	char ssid[32];
	char cmd[256];

	/* TODO: build message using odid_wifi_build_* commands
	 * and send them out using hostapd commands */

	snprintf(ssid, sizeof(ssid), "ID LAT%3.6f LON%3.6f", drone->latitude, drone->longitude);

	printf("set SSID to %s, %d\n", ssid, (int)strlen(ssid));

	system("hostapd_cli DISABLE");
	snprintf(cmd, sizeof(cmd), "hostapd_cli SET ssid \"%s\"", ssid);
	system(cmd);
	system("hostapd_cli ENABLE");
}


int main(int argc, char *argv[])
{
	struct odid_drone_information drone;
	struct global global;
	struct gps_data_t gpsdata;
	int ret, errno;

	memset(&drone, 0, sizeof(drone));
	memset(&global, 0, sizeof(global));

	if (read_arguments(argc, argv, &drone, &global) < 0) {
		usage(argv[0]);
		return -1;
	}

	if (gps_open(global.server, global.port, &gpsdata) != 0) {
		fprintf(stderr, "%s: gpsd error: %d, %s\n", argv[0],
			errno, gps_errstr(errno));
		return -1;
	}

	gps_stream(&gpsdata, WATCH_ENABLE | WATCH_JSON, NULL);

	/* polling interval is once a second */
	while (1) {
		sleep(1);

		/* read as much as we can using gps_read() */
#if GPSD_API_MAJOR_VERSION >= 7
		while ((ret = gps_read(&gpsdata, NULL, 0)) > 0);
#else
		while ((ret = gps_read(&gpsdata)) > 0);
#endif
		if (ret < 0) {
			fprintf(stderr, "%s: gpsd_read error: %d, %s\n", argv[0],
				errno, gps_errstr(errno));
		}

		drone_adopt_gps_data(&drone, &gpsdata);
		drone_send_data(&drone);
 	}

	gps_stream(&gpsdata, WATCH_DISABLE, NULL);
	gps_close(&gpsdata);
}

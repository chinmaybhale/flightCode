// HELIOS ROCKETRY
// Code here will only run once in the start
// Think setup() from arduino

#include "headers.h"
static void init_sensor(char *, struct sensor *);
static void init_valve(char *, struct valve *);

static void read_config(char *file_name)
{
	// CSV FORMAT
	// sensor list will start with ###SENSORS###
	// valve list will start with ###VALVES###
	//
	// sensor_name(7)[0-6], base_val(4)[8-11], +ve error(2)[13, 14], -ve error(2)[16, 17], pin# (if availabe, -1 otherwise)(2)[19, 20]
	// or
	// valve_name(7)[0-6], pin#(2)[8, 9]
	//
	// sensor_name will decide if it is a pressure or temperature or other kind of sensor.
	
	int i;
	char *setup = (char *)malloc(sizeof(char) * 21);
	FILE *file = fopen(file_name, "wr");

	if(!file) {
		printf("Unable to open file\n");
		return;
	}
	
	while(fgets(setup, 13, file)) {
		setup[14] = '\0';

		if(setup[0] == '#' && setup[3] == 'S') {
			for(i = 0; i < 9; i++) {
				fgets(setup, 20, file);
				setup[20] = '\0';
				init_sensor(setup, &s[i]);
			}		
		}
		else if (setup[0] == '#' && setup[3] == 'V') {
			for(i = 0; i < 13; i++) {
				fgets(setup, 9, file);
				setup[9] = '\0';
				init_valve(setup, &v[i]);
			}
		}
	}

	fclose(file);
	return;
}

static void init_sensor(char *setup, struct sensor *p)
{
	char *str_base_val = (char *)malloc(sizeof(char) * 5);
	char *str_pos_err = (char *)malloc(sizeof(char) * 3);
	char *str_neg_err = (char *)malloc(sizeof(char) * 3);
	char *str_pin = (char *)malloc(sizeof(char) * 3);

	strncpy(str_base_val, setup + 8, 4);
	str_base_val[4] = '\0';

	strncpy(str_pos_err, setup + 13, 2);
	str_pos_err[2] = '\0';

	strncpy(str_neg_err, setup + 16, 2);
	str_neg_err[2] = '\0';

	strncpy(str_pin, setup + 19, 2);
	str_pin[2] = '\0';
	
	float base_val = atof(str_base_val);
	float pos_err = atof(str_pos_err) / 100.0f;
	float neg_err = atof(str_pos_err) / 100.0f;
	int pin = atoi(str_pin);

	p->max_val = base_val + (base_val * pos_err);
	p->min_val = base_val - (base_val * neg_err);
	p->pin = pin;
	
	free(str_base_val);
	free(str_pos_err);
	free(str_neg_err);
	free(str_pin);

	return;
}

static void init_valve(char *setup, struct valve *v)
{
	char *str_pin = (char *)malloc(sizeof(char) * 3);

	strncpy(str_pin, setup + 8, 2);
	str_pin[2] = '\0';

	v->pin = atoi(str_pin);
	v->stat = 0;

	free(str_pin);

	return;
}

static void system_check()
{
	return;
}

void init(char *file_name)
{
	read_config(file_name);
	system_check();
	return;
}

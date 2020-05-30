// HELIOS ROCKETRY
// Code here will only run once in the start
// Think setup() from arduino

#include "headers.h"
static void init_sensor(char *, struct sensor *);
static void init_valve(char *, struct valve *);
static struct sensor *get_init_values(char *);

static void show_config(char *file_name)
{
	// This function shows the config which has been set from the config file. Code will not execute further
	// without conformation from user that configuration is set right.
	char opt;
	int i;
	FILE *file = fopen(file_name, "r");
	char *name = (char *)malloc(sizeof(char) * 20);
	struct sensor *init_s = get_init_values(file_name);

	if(!file) {
		printf("Not able to open file!");
		return;
	}

	printf("\n############### HELIOS ROCKETRY ##############\n");
	printf("###### Configuration of firing sequence ######\n\n");
	printf(">>> SENSOR CONFIGURATION <<<\n\n");
	
	// reading line ###SENSORS###
	fgets(name, 20, file);
	
	printf("*** PRE-SEQUENCE CONFIGURATION ***\n");
	printf("SENSOR NAME\t|\tMAX VALUE\t|\tMIN VALUE\t|\tPIN\t|\n");
	printf("---------------------------------------------------------------------------------\n");
	
	// reading line >INTIALIZATION
	fgets(name, 20, file);

	for(i = 0; i < SENSOR_NUM; i++) {
		fgets(name, 8, file);
		name[8] = '\0';
		printf("%s\t\t|\t%f\t|\t%f\t|\t%d\t|\n", name, init_s[i].max_val, init_s[i].min_val, init_s[i].pin);
		fgets(name, 20, file);
	}
	
	printf("\n*** MAIN SEQUENCE CONFIGURATION ***\n");
	printf("SENSOR NAME\t|\tMAX VALUE\t|\tMIN VALUE\t|\tPIN\t|\n");
	printf("---------------------------------------------------------------------------------\n");
	
	// reading line >MAIN
	fgets(name, 20, file);

	for(i = 0; i < SENSOR_NUM; i++) {
		fgets(name, 8, file);
		name[8] = '\0';
		printf("%s\t\t|\t%f\t|\t%f\t|\t%d\t|\n", name, s[i].max_val, s[i].min_val, s[i].pin);
		fgets(name, 20, file);
	}

	printf("\n>>> VALVE CONFIGURATION <<<\n\n");
	printf("VALVE NAME\t|\tPIN\t|\tSTATUS\t|\n");
	printf("-------------------------------------------------\n");

	fgets(name, 20, file);

	for(i = 0; i < VALVE_NUM; i++) {
		fgets(name, 8, file);
		name[8] = '\0';
		printf("%s\t\t|\t%d\t|\t%s\t|\n", name, v[i].pin, ((v[i].stat)? "on" : "off" ));
		fgets(name, 20, file);
	}

	printf("\n>>> DEBUG CONFIGURATION <<<\n\n");
	printf("DEBUG: %s\n", ((debug)? "YES" : "NO"));

	printf("\n\nConfirm configuration (y/N): ");
	scanf("%c", &opt);

	if(opt != 'y') {
		printf("Exiting!\n");
		free(name);
		fclose(file);
		exit(EXIT_FAILURE);
	}

	free(name);
	fclose(file);
	return;
}

static void read_config(char *file_name)
{
	// CSV FORMAT
	// sensor list will start with ###SENSORS###
	// valve list will start with ###VALVES###
	// debug will start with ###DEBUG###
	// 
	// sensor_name(7)[0-6], base_val(4)[8-11], +ve error(2)[13, 14], -ve error(2)[16, 17], pin# (if availabe, -1 otherwise)(2)[19, 20]
	// or
	// valve_name(7)[0-6], pin#(2)[8, 9]
	//
	// sensor_name will decide if it is a pressure or temperature or other kind of sensor.
	
	/* EXAMPLE CONFIG FILE
	 *
	 * ###SENSORS###
	 * N_PT_01,1000,10,5,-1
	 * R_PT_01,500,10,10,-1
	 * ###VALVES###
	 * N_SV_01,0
	 * ###DEBUG###
	 * 1
	 */
	
	int i;
	char *setup = (char *)malloc(sizeof(char) * 25);
	FILE *file = fopen(file_name, "r");

	if(!file) {
		printf("Unable to open file\n");
		return;
	}
	
	while(fgets(setup, 23, file)) {
		setup[23] = '\0';

		if(setup[0] == '>' && setup[1] == 'M') {
			for(i = 0; i < SENSOR_NUM; i++) {
				fgets(setup, 23, file);
				setup[23] = '\0';
				init_sensor(setup, &s[i]);
			}
		}
		else if (setup[0] == '#' && setup[3] == 'V') {
			for(i = 0; i < VALVE_NUM; i++) {
				fgets(setup, 11, file);
				setup[11] = '\0';
				init_valve(setup, &v[i]);
			}
		}
		else if (setup[0] == '#' && setup[3] == 'D') {
			fgets(setup, 2, file);
			if(setup[0] == '1')
				debug = 1;
			else
				debug = 0;
		}
	}

	free(setup);
	fclose(file);
	return;
}

struct sensor *get_init_values(char *file_name)
{
	struct sensor *init_s = (struct sensor *)malloc(sizeof(struct sensor) * SENSOR_NUM);
	FILE *file = fopen(file_name, "r");
	char *setup = (char *)malloc(sizeof(char) * 25);
	int i;

	while(fgets(setup, 23, file)) {
		setup[23] = '\0';

		if(setup[0] == '>' && setup[1] == 'I') {
			for(i = 0; i < SENSOR_NUM; i++) {
				fgets(setup, 23, file);
				setup[23] = '\0';
				init_sensor(setup, &init_s[i]);
			}

			break;
		}
	}

	
	free(setup);
	fclose(file);
	return init_s;
}

static void init_sensor(char *setup, struct sensor *s)
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

	s->max_val = base_val + (base_val * pos_err);
	s->min_val = base_val - (base_val * neg_err);
	s->pin = pin;
	
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


static void system_check(char *file_name)
{
	struct sensor *init_s = get_init_values(file_name);
	get_data(); // get the most current values from the DAQ
	//TODO:Should we add the if statement here if there is a verbose var?
	
	//TODO: Check P1
	
	if(init_s[L_PT_02].max_val > 4500)
	{
		printf("ERROR: L_PT_02 is not less than static pressure.");
		printf("VALUE: %f", init_s[L_PT_02].max_val);
	}

	if(init_s[L_PT_03].max_val > 4500)
	{
		printf("ERROR: L_PT_03 is not less than static pressure.");
		printf("VALUE: %f", init_s[L_PT_02].max_val);
	}

	if(init_s[R_PT_02].max_val > 4500)
	{
		printf("ERROR: R_PT_02 is not less than static pressure.");
		printf("VALUE: %f", init_s[L_PT_02].max_val);
	}

	if(init_s[R_PT_03].max_val > 4500)
	{
		printf("ERROR: L_PT_02 is not less than static pressure.");
		printf("VALUE: %f", init_s[R_PT_02].max_val);
	}

	//TODO: read Liquid indicator here

	//TODO: Make sure the expexted value is off
	if(v[L_SV_03].stat != 0)
	{
		printf("ERROR: L_SV_03 is not 0");
		printf("VALUE: %f", v[L_SV_03].stat);
	}

	if(init_s[L_PT_01].max_val != 0)
	{
		printf("ERROR: L_PT_01 is not 0");
		printf("VALUE: %f", init_s[L_PT_01].max_val);
	}
	if(init_s[R_PT_01].max_val != 0)
	{
		printf("ERROR: R_PT_01 is not 0");
		printf("VALUE: %f", init_s[R_PT_01].max_val);
	}

	//TODO: Change the expected value to ambient temp
	if(init_s[L_TT_01].max_val != 0)
	{
		printf("ERROR: L_TT_01 is not 0");
		printf("VALUE: %f", init_s[L_TT_01].max_val);
	}
	
	//TODO: Change the expected value to ambient temp
	if(init_s[R_TT_01].max_val != 0)
	{
		printf("ERROR: R_TT_01 is not 0");
		printf("VALUE: %f", init_s[L_PT_01].max_val);
	}

	//TODO: Need expected value for P3
	if(v[R_PV_05].stat != 0)
	{
		printf("R_PV_05 is not off");
		printf("Value: %f", v[R_PV_03].stat);
	}


	return;
}

void init(char *file_name)
{
	read_config(file_name);
	show_config(file_name);
	if(debug)
		init_file();	
	system_check(file_name);
	return;
}

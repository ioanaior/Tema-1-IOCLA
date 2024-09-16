#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// folosesc structurile din structs.h
#include "structs.h"

// declar antetul pentru functia get_operations
void get_operations(void **operations);

/* functia main are parametri necesari pentru a
primi argumente in linia de comanda */
int main(int argc, char const *argv[])
{

	// operations = vectorul de pointeri la functiile din operations.c
	static void (*operations[8])(void *);
	get_operations((void **)operations);
	// fisierul binar este primit ca argumentul cu numarul 1
	char const *fisier = argv[1];
	FILE *f = fopen(fisier, "rb");

	sensor *v, *aux;
	int nr_sensors, i, j, k, nr = -1, ok;
	int type;
	char s[20];
	fread(&nr_sensors, sizeof(int), 1, f);
	// alocarea dinamica a vectorului de senzori v
	v = (sensor *)malloc((nr_sensors) * sizeof(sensor));
	// si a vectorului aux cu ajutorul caruia voi realiza sortarea
	aux = (sensor *)malloc((nr_sensors) * sizeof(sensor));

	// se citesc datele despre senzori din fisierul binar
	for (i = 0; i < nr_sensors; i++) {
		fread(&type, sizeof(int), 1, f);
		// daca senzorul este de tip 1 (PMU)
		if (type == 1) {
			v[i].sensor_type = PMU;
			// se aloca si campul data al senzorului
			v[i].sensor_data = (power_management_unit *)malloc
			(sizeof(power_management_unit));
			fread(v[i].sensor_data, sizeof(power_management_unit), 1, f);
		// daca senzorul este de tip 0 (TIRE)
		} else if (type == 0) {
			v[i].sensor_type = TIRE;
			v[i].sensor_data = (tire_sensor *)malloc(sizeof(tire_sensor));
			fread(v[i].sensor_data, sizeof(tire_sensor), 1, f);
		}
		fread(&v[i].nr_operations, sizeof(int), 1, f);
		// se aloca vectorul de operatii ca int*
		v[i].operations_idxs = (int *)malloc
		((v[i].nr_operations) * sizeof(int));
		for (j = 0; j < v[i].nr_operations; j++)
			fread(&v[i].operations_idxs[j], sizeof(int), 1, f);
		// senzorii de tip PMU se introduc in ordine in aux
		if (v[i].sensor_type == 1) {
			aux[++nr] = v[i];
		}
	}
	// v se parcurge din nou si se introduc in aux si senzorii de tip TIRE
	for (i = 0; i < nr_sensors; i++)
		if (v[i].sensor_type == 0) {
			aux[++nr] = v[i];
		}
	// v(nesortat) se elibereaza
	free(v);
	// si devine aux
	v = aux;

	// se citesc pe rand comenzile in stringul s
	scanf("%s", s);
	while (strcmp(s, "exit") != 0) {
		scanf("%d", &i);
		// daca indicele este valid
		if (i >= 0 && i < nr_sensors) {
			// pentru operatia de print
			if (strcmp(s, "print") == 0) {
				// daca senzorul i este de tip PMU
				if (v[i].sensor_type == 1) {
					// se afiseaza informatiile conform indicatiilor din enunt
					printf("Power Management Unit\n");
					printf("Voltage: %.2f\n", (*(power_management_unit *)
					v[i].sensor_data).voltage);
					printf("Current: %.2f\n", (*(power_management_unit *)
					v[i].sensor_data).current);
					printf("Power Consumption: %.2f\n",
					(*(power_management_unit *)
					v[i].sensor_data).power_consumption);
					printf("Energy Regen: %d%%\n",
					(*(power_management_unit *)
					v[i].sensor_data).energy_regen);
					printf("Energy Storage: %d%%\n",
					(*(power_management_unit *)
					v[i].sensor_data).energy_storage);
				// daca senzorul i este de tip TIRE
				} else if (v[i].sensor_type == 0) {
					printf("Tire Sensor\n");
					printf("Pressure: %.2f\n", (*(tire_sensor *)
					v[i].sensor_data).pressure);
					printf("Temperature: %.2f\n", (*(tire_sensor *)
					v[i].sensor_data).temperature);
					printf("Wear Level: %d%%\n", (*(tire_sensor *)
					v[i].sensor_data).wear_level);
					// daca performance score este pozitiv se afiseaza
					if ((*(tire_sensor *)v[i].sensor_data).
					performace_score > 0)
						printf("Performance Score: %d\n", (*(tire_sensor *)
						v[i].sensor_data).performace_score);
					// altfel se considera necalculat
					else
						printf("Performance Score: Not Calculated\n");
				}
			}
			// daca se citeste comanda analyze
			if (strcmp(s, "analyze") == 0) {
				// se parcurge vectorul de operatii al senzorului i
				for (j = 0; j < v[i].nr_operations; j++) {
					// si se executa operatiile
					(*operations[v[i].operations_idxs[j]])(v[i].sensor_data);
				}
			}
			// daca se citeste comanda clear
			if (strcmp(s, "clear") == 0) {
				// vectorul de senzori de parcurge in sens invers
				for (k = nr_sensors - 1; k >= 0; k--) {
					// ok - marcheaza validitatea datelor senzorului k
					ok = 1;
					if (v[k].sensor_type == 0) {
						if ((*(tire_sensor *)v[k].sensor_data).pressure < 19 ||
							(*(tire_sensor *)v[k].sensor_data).pressure > 28)
							ok = 0;
						if ((*(tire_sensor *)v[k].sensor_data).temperature
							< 0 ||
							(*(tire_sensor *)v[k].sensor_data).temperature
							> 120)
							ok = 0;
						if ((*(tire_sensor *)v[k].sensor_data).wear_level
							< 0 ||
							(*(tire_sensor *)v[k].sensor_data).wear_level
							> 100)
							ok = 0;
					}
					if (v[k].sensor_type == 1) {
						if ((*(power_management_unit *)v[k].sensor_data).
							voltage < 10 ||
							(*(power_management_unit *)v[k].sensor_data).
							voltage > 20)
							ok = 0;
						if ((*(power_management_unit *)v[k].sensor_data).
							current < -100 ||
							(*(power_management_unit *)v[k].sensor_data).
							current > 100)
							ok = 0;
						if ((*(power_management_unit *)v[k].sensor_data).
							power_consumption < 0 ||
							(*(power_management_unit *)v[k].sensor_data).
							power_consumption > 1000)
							ok = 0;
						if ((*(power_management_unit *)v[k].sensor_data).
							energy_regen < 0 ||
							(*(power_management_unit *)v[k].sensor_data).
							energy_regen > 100)
							ok = 0;
						if ((*(power_management_unit *)v[k].sensor_data).
							energy_storage < 0 ||
							(*(power_management_unit *)v[k].sensor_data).
							energy_storage > 100)
							ok = 0;
					}
					// dupa verificari ok va deveni 0 daca exista date invalide
					if (ok == 0) {
						/* eliminam senzorul k;
						mai intai se elibereaza campurile de sensors_data
						si operations_idxs */
						free(v[k].sensor_data);
						free(v[k].operations_idxs);
						// senzorii de dupa k se muta cu o pozitie la stanga
						for (j = k; j < nr_sensors - 1; j++) {
							v[j] = v[j + 1];
						}
						// numarul de senzori va scadea cu 1
						nr_sensors--;
						// se realoca vectorul de senzori cu noua lungime
						v = (sensor *)realloc(v, nr_sensors * sizeof(sensor));
					}
				}
			}
		// daca indicele i este in afara range-ului se afiseaza un mesaj
		} else
			printf("Index not in range!\n");
		// se citeste urmatoarea comanda
		scanf("%s", s);
	}

	// se elibereaza campurile alocate pentru senzorii din vectorul v
	for (i = 0; i < nr_sensors; i++) {
		free(v[i].sensor_data);
		free(v[i].operations_idxs);
	}
	// se elibereaza vectorul de senzori
	free(v);
	// se inchide fisierul .dat
	fclose(f);
	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct {
	int x, y;
} Vec2;

int choose_optimal_arrival(int err1, int err2);
int dist_calc(Vec2 start, Vec2 end);
int arrival_calc(Vec2 startPos, int startTime, Vec2 endPos, int endTime);
int digit_count(int d);

int main() {
	int rowCount, columnCount;
	int vehicleCount;
	int rideCount;
	int bonus;
	int time;

	char** result = malloc(sizeof(char*) * vehicleCount);
	for (int v = 0; v < vehicleCount; v++) {
		result[v] = malloc(sizeof(char) * rideCount * digit_count(rideCount));
		result[v][0] = '\0';
	}

	if (scanf("%i %i %i %i %i %i\n",
		&rowCount,
		&columnCount,
		&vehicleCount,
		&rideCount,
		&bonus,
		&time) != 6) {
		fprintf(stderr, "bad input\n");
		exit(1);
	}

	Vec2* rideStartPos = malloc(sizeof(Vec2) * rideCount);
	Vec2* rideEndPos = malloc(sizeof(Vec2) * rideCount);
	int* rideStartTime = malloc(sizeof(int) * rideCount);
	int* rideEndTime = malloc(sizeof(int) * rideCount);

	for(int i = 0; i < rideCount; i++) {
		if (scanf("%i %i %i %i %i %i\n",
			&rideStartPos[i].x,
			&rideStartPos[i].y,
			&rideEndPos[i].x,
			&rideEndPos[i].y,
			&rideStartTime[i],
			&rideEndTime[i]) != 6) {
			fprintf(stderr, "bad input\n");
			exit(1);
		}
	}

	Vec2* vehiclePos = malloc(sizeof(Vec2) * vehicleCount);
	int* vehicleTime = malloc(sizeof(int) * vehicleCount);

	for (int i = 0; i < vehicleCount; i++) {
		vehiclePos[i].x = 0;
		vehiclePos[i].y = 0;
		vehicleTime[i] = 0;
	}
	
	printf("arrivals by begin (1D)\n");
	for (int r = 0; r < rideCount; r++) {
		printf("%i ", arrival_calc((Vec2){0, 0}, 0, rideStartPos[r], rideStartTime[r]));
	}
	printf("\n");

	printf("completions by begin (1D)\n");
	for (int r = 0; r < rideCount; r++) {
		int arrival = arrival_calc((Vec2){0, 0}, 0, rideStartPos[r], rideStartTime[r]);
		int delay = arrival < 0 ? 0 : arrival;
		printf("%i ", arrival_calc(
			rideStartPos[r], 
			rideStartTime[r],
			rideEndPos[r], 
			rideEndTime[r]) + delay);
	}
	printf("\n");

	printf("arrivals by previous (2D)\n");
	for (int p = 0; p < rideCount; p++) {
		for (int r = 0; r < rideCount; r++) {
			printf("%i ", arrival_calc(
				rideEndPos[p], 
				rideEndTime[p],
				rideStartPos[r], 
				rideStartTime[r]));
		}
		printf("\n");
	}

	for (int r = 0; r < rideCount; r++) {
		int choosen_v = 0;
		for (int v = 1; v < vehicleCount; v++) {
			printf("choosen_v: %i\n", choosen_v);
			int arrival_current = arrival_calc(
				vehiclePos[v], 
				vehicleTime[v],
				rideStartPos[r], 
				rideStartTime[r]);
			int arrival_opt = arrival_calc(
				vehiclePos[choosen_v],
				vehicleTime[choosen_v],
				rideStartPos[r],
				rideStartTime[r]);

			int arrival = choose_optimal_arrival(arrival_current, arrival_opt);
			if (arrival_current == arrival)
				choosen_v = v;
		}
		printf("choosen_v: %i\n", choosen_v);

		int arrival_to_start = arrival_calc(
			vehiclePos[choosen_v],
			vehicleTime[choosen_v],
			rideStartPos[r],
			rideStartTime[r]);

		int arrival_to_end = arrival_calc(
			rideStartPos[r],
			rideStartTime[r],
			rideEndPos[r],
			rideEndTime[r]);

		int delay = arrival_to_start < 0 ? 0 : arrival_to_start;

		vehicleTime[choosen_v] = rideEndTime[r] + arrival_to_end + delay;
		vehiclePos[choosen_v] = rideEndPos[r];
		
		printf("hello world\n");
		printf("%i\n", choosen_v);
		int count = strlen(result[choosen_v]);
		printf("count: %i\n", count);
		sprintf(result[choosen_v] + count, "%i ", r);
	}

	// print
	for (int v = 0; v < vehicleCount; v++) {
		printf("%s\n", result[v]);
	}

	return 0;
}

int choose_optimal_arrival(int err1, int err2) {
	if (err1 > 0) {
		if (err2 > 0) {
			if (err1 < err2)
				return err1;
			return err2;
		}
		if (err2 < 0)
			return err2;
		return err2;
	}

	if (err1 < 0) {
		if (err2 < 0) {
			if (err1 > err2)
				return err1;
			return err2;
		}
		if (err2 > 0)
			return err2;
		return err2;
	}

	if (err1 == 0)
		return err1;
}

int dist_calc(Vec2 start, Vec2 end) {
	return abs(end.x - start.x) + abs(end.y - start.y);
}

int arrival_calc(Vec2 startPos, int startTime, Vec2 endPos, int endTime) {
	int dist = dist_calc(startPos, endPos);
	int time = endTime - startTime;
	return dist - time;
}

int digit_count(int d) {
	char snum[100];
	sprintf(snum, "%i", d);
	return strlen(snum);
}

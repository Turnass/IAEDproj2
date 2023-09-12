#ifndef PROJ2
#define PROJ2

#define DELIMITER " \t\n"
#define MAX_INPUT 65535
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct stop stop;
typedef struct route route;
typedef struct con con;

struct stop {
    char *name;
    double lat;
    double log;
    int n_routes;
    stop *link;
    route **routes;
};

struct route {
    int n_stops;
    double cost;
    double duration;
    char *name;
    con **route_cons;
    route *link;
};

struct con {
    route *r;
    stop *p1;
    stop *p2;
    double cost;
    double duration;
};

stop *get_stop(char *s, stop *head_stop);

void add_stop(stop *head_stop, stop *new_stop);

void stops(char str[MAX_INPUT], stop **head_stop);

route *get_route(char *c, route *head_route);

void add_route(route *head_route, route *new_route);

void routes(char str[MAX_INPUT], route **head_route);

void connects(char str[MAX_INPUT], route *head_route, stop *head_stop);

void intersc(stop *head_stop);

void shut_down();

void delete_stop_connections(stop *p, route *r);

void stop_routes(route *head_route, stop *s);

void delete_stop(char *name, stop **head_stop);

void delete_route_cons(route *c);

void delete_route(char *name, route **head_route, stop *head_stop);

char *get_name(char str[MAX_INPUT]);

void kill(stop **head_stop, route **head_route);

void destino(char *name, stop *head_stop, route *head_route);

#endif /* !PROJ2 */
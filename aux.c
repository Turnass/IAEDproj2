#include "project2.h"

/***** STOP RELATED *****/

stop *get_stop(char *name, stop *head_stop) {
    stop *stop_ptr;
    stop_ptr = head_stop;
    while (stop_ptr != NULL) {
        if (strcmp(name, stop_ptr->name) == 0)
            return stop_ptr;
        stop_ptr = stop_ptr->link;
    }
    return NULL;
}

void add_stop(stop *head_stop, stop *new_stop) {
    stop *stop_ptr;
    stop_ptr = head_stop;
    while (stop_ptr->link)
        stop_ptr = stop_ptr->link;
    stop_ptr->link = new_stop;
}

/***** ROUTE RELATED *****/

route *get_route(char *name, route *head_route) {
    route *route_ptr;
    route_ptr = head_route;
    while (route_ptr) {
        if (strcmp(name, route_ptr->name) == 0)
            return route_ptr;
        route_ptr = route_ptr->link;
    }
    return NULL;
}

void add_route(route *head_route, route *new_route) {
    route *route_ptr;
    route_ptr = head_route;
    while (route_ptr->link)
        route_ptr = route_ptr->link;
    route_ptr->link = new_route;
}

/***** STOP DELETION RELATED *****/

void delete_stop_connections(stop *p, route *r) {
    int i, j;
    while (r->n_stops != 0 && r->route_cons[0]->p1 == p) {
        r->cost -= r->route_cons[0]->cost;
        r->duration -= r->route_cons[0]->duration;
        if (r->n_stops == 2) {
            r->n_stops = 0;
            if (p != r->route_cons[0]->p2) {
                for (i = 0; i < r->route_cons[0]->p2->n_routes; i++)
                    if (r->route_cons[0]->p2->routes[i] == r) {
                        for (j = i; j < r->route_cons[0]->p2->n_routes - 1; j++)
                            r->route_cons[0]->p2->routes[j] =
                                r->route_cons[0]->p2->routes[j + 1];
                        r->route_cons[0]->p2->routes =
                            realloc(r->route_cons[0]->p2->routes,
                                    (r->route_cons[0]->p2->n_routes - 1) *
                                        sizeof(route *));
                        break;
                    }
                r->route_cons[0]->p2->n_routes--;
            }
            free(r->route_cons[0]);
            r->route_cons[0] = NULL;
        } else {
            free(r->route_cons[0]);
            for (i = 0; i < r->n_stops - 2; i++)
                r->route_cons[i] = r->route_cons[i + 1];
            r->n_stops--;
        }
    }

    while (r->n_stops != 0 && r->route_cons[r->n_stops - 2]->p2 == p) {
        r->cost -= r->route_cons[r->n_stops - 2]->cost;
        r->duration -= r->route_cons[r->n_stops - 2]->duration;
        if (r->n_stops == 2) {
            if (r->route_cons[0]->p1 != p) {
                for (i = 0; i < r->route_cons[0]->p1->n_routes; i++)
                    if (r->route_cons[0]->p1->routes[i] == r) {
                        for (j = i; j < r->route_cons[0]->p1->n_routes - 1; j++)
                            r->route_cons[0]->p1->routes[j] =
                                r->route_cons[0]->p1->routes[j + 1];
                        r->route_cons[0]->p1->routes =
                            realloc(r->route_cons[0]->p1->routes,
                                    (r->route_cons[0]->p1->n_routes - 1) *
                                        sizeof(route *));
                        break;
                    }
                r->route_cons[0]->p1->n_routes--;
            }
            free(r->route_cons[r->n_stops - 2]);
            r->n_stops = 0;
        } else {
            free(r->route_cons[r->n_stops - 2]);
            r->n_stops--;
        }
    }

    for (i = 0; i < r->n_stops - 2; i++) {
        while (r->route_cons[i]->p2 == p) {
            r->route_cons[i + 1]->p1 = r->route_cons[i]->p1;
            r->route_cons[i + 1]->cost += r->route_cons[i]->cost;
            r->route_cons[i + 1]->duration += r->route_cons[i]->duration;
            free(r->route_cons[i]);
            for (j = i; j < r->n_stops - 2; j++)
                r->route_cons[j] = r->route_cons[j + 1];
            r->n_stops--;
        }
    }
    r->route_cons = realloc(r->route_cons, sizeof(con *) * r->n_stops);
    p->n_routes--;
}

/***** ROUTE DELETION RELATED *****/

void delete_route_cons(route *c) {
    int i;
    for (i = 0; i < c->n_stops - 1; i++)
        free(c->route_cons[i]);
    free(c->route_cons);
}

/***** OTHERS *****/

char *get_name(char str[MAX_INPUT]) {
    char *name;

    if (strchr(str, '"')) {
        strtok(str, "\"");
        name = strtok(NULL, "\"");
    } else
        name = strtok(str, DELIMITER);

    return name;
}

void shut_down() {
    printf("No memory.");
    exit(0);
}
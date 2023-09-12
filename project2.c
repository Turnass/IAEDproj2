/* iaed-23 - ist1106658 - project2 */

#include "project2.h"

int main() {
    char first_char, *name;
    stop *head_stop;
    route *head_route;

    head_stop = NULL;

    head_route = NULL;

    while ((first_char = getchar()) != 'q') {
        char str[MAX_INPUT];
        fgets(str, MAX_INPUT, stdin);
        switch (first_char) {
        case 'c':
            routes(str, &head_route);
            break;
        case 'p':
            stops(str, &head_stop);
            break;
        case 'l':
            connects(str, head_route, head_stop);
            break;
        case 'i':
            intersc(head_stop);
            break;
        case 'e':
            name = get_name(str);
            delete_stop(name, &head_stop);
            break;
        case 'r':
            name = get_name(str);
            delete_route(name, &head_route, head_stop);
            break;
        case 'a':
            kill(&head_stop, &head_route);
            break;
        case 'f':
            name = get_name(str);
            destino(name, head_stop, head_route);
            break;
        }
    }
    kill(&head_stop, &head_route);
    return 0;
}

void stops(char str[MAX_INPUT], stop **head_stop) {
    char *token, *name;
    stop *current_stop, *stop_ptr;
    current_stop = (stop *)malloc(sizeof(stop));
    if (current_stop == NULL)
        shut_down();

    stop_ptr = *head_stop;

    if (strchr(str, '"')) { /*In case <stop_name> is in quoats, the token is
                               everything within them*/
        strtok(str, "\"");
        name = strtok(NULL, "\"");
    } else
        name = strtok(str, DELIMITER);

    if (name == NULL) { /*input = p*/
        while (stop_ptr != NULL) {
            printf("%s: %16.12f %16.12f %d\n", stop_ptr->name, stop_ptr->lat,
                   stop_ptr->log, stop_ptr->n_routes);
            stop_ptr = stop_ptr->link;
        }
        free(current_stop);
        return;
    }

    current_stop->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
    if (current_stop->name == NULL)
        shut_down();
    strcpy(current_stop->name, name);

    token = strtok(NULL, DELIMITER);

    if (token == NULL) { /*input = p <stop_name>*/
        if (get_stop(current_stop->name, *head_stop) == NULL)
            printf("%s: no such stop.\n", current_stop->name);
        else
            printf("%16.12f %16.12f\n",
                   get_stop(current_stop->name, *head_stop)->lat,
                   get_stop(current_stop->name, *head_stop)->log);

        free(current_stop->name);
        free(current_stop);
        return;
    }

    current_stop->lat = atof(token);

    token = strtok(NULL, DELIMITER);
    current_stop->log = atof(token);

    if (current_stop->lat < -90 || current_stop->lat > 90 || current_stop->log < -180 || current_stop->log > 180){
        printf("invalid location.\n");
        free(current_stop->name);
        free(current_stop);
        return;
    }
    
    current_stop->n_routes = 0;
    current_stop->link = NULL;
    current_stop->routes = NULL;

    if (get_stop(current_stop->name, *head_stop) != NULL) {
        printf("%s: stop already exists.\n", current_stop->name);
        free(current_stop->name);
        free(current_stop);
        return;
    }

    if (!*head_stop) {
        *head_stop = current_stop;
    } else
        add_stop(*head_stop, current_stop);
}

void routes(char str[MAX_INPUT], route **head_route) {
    char *token, *name;
    route *current_route, *route_ptr;
    int i;

    current_route = (route *)malloc(sizeof(route));
    if (current_route == NULL)
        shut_down();
    route_ptr = *head_route;
    name = strtok(str, DELIMITER);

    if (name == NULL) {
        while (route_ptr != NULL) {
            if(strlen(route_ptr->name) < 6){
                if (route_ptr->n_stops != 0)
                    printf("%s %s %s %d %.2f %.2f\n", route_ptr->name,
                        route_ptr->route_cons[0]->p1->name,
                        route_ptr->route_cons[route_ptr->n_stops - 2]->p2->name,
                        route_ptr->n_stops, route_ptr->cost,
                        route_ptr->duration);
                else
                    printf("%s %d %.2f %.2f\n", route_ptr->name, route_ptr->n_stops,
                        route_ptr->cost, route_ptr->duration);
            }
            route_ptr = route_ptr->link;
        }
        free(current_route);
        return;
    }
    current_route->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
    if (current_route->name == NULL)
        shut_down();
    strcpy(current_route->name, name);

    current_route->cost = 0;
    current_route->duration = 0;
    current_route->n_stops = 0;
    current_route->route_cons = NULL;
    current_route->link = NULL;

    token = strtok(NULL, DELIMITER);

    if (token == NULL) {
        if (!*head_route) {
            *head_route = current_route;
        } else if (get_route(current_route->name, *head_route) == NULL)
            add_route(*head_route, current_route);
        else {
            if (get_route(current_route->name, *head_route)->n_stops)
                for (i = 0;
                     i <
                     get_route(current_route->name, *head_route)->n_stops - 1;
                     i++) {
                    if (i ==
                        get_route(current_route->name, *head_route)->n_stops -
                            2) {
                        printf("%s, ",
                               get_route(current_route->name, *head_route)
                                   ->route_cons[i]
                                   ->p1->name);
                        printf("%s\n",
                               get_route(current_route->name, *head_route)
                                   ->route_cons[i]
                                   ->p2->name);
                    } else
                        printf("%s, ",
                               get_route(current_route->name, *head_route)
                                   ->route_cons[i]
                                   ->p1->name);
                }
            free(current_route->name);
            free(current_route);
        }
    } else if (strcmp(token, "inverso") == 0 || strcmp(token, "inv") == 0 ||
               strcmp(token, "inve") == 0 || strcmp(token, "inver") == 0 ||
               strcmp(token, "invers") == 0) {
        if (get_route(current_route->name, *head_route)->n_stops)
            for (i = get_route(current_route->name, *head_route)->n_stops - 2;
                 i >= 0; i--) {
                if (i == 0) {
                    printf("%s, ", get_route(current_route->name, *head_route)
                                       ->route_cons[i]
                                       ->p2->name);
                    printf("%s\n", get_route(current_route->name, *head_route)
                                       ->route_cons[i]
                                       ->p1->name);
                } else
                    printf("%s, ", get_route(current_route->name, *head_route)
                                       ->route_cons[i]
                                       ->p2->name);
            }
        free(current_route->name);
        free(current_route);
    } else {
        printf("incorrect sort option.\n");
        free(current_route->name);
        free(current_route);
    }
}

void connects(char str[MAX_INPUT], route *head_route, stop *head_stop) {
    char *c_name, *p1_name, *p2_name, *cost, *dur;
    int i, is_con0;
    route *c;
    stop *p1, *p2;
    con *link;

    c_name = strtok(str, DELIMITER);
    if (get_route(c_name, head_route) == NULL) {
        printf("%s: no such line.\n", c_name);
        return;
    }
    c = get_route(c_name, head_route);

    if (str[strlen(c_name) + 2] == '"') {
        p1_name = strtok(NULL, "\"");
        i = strlen(c_name) + strlen(p1_name) + 6;
    } else {
        p1_name = strtok(NULL, DELIMITER);
        i = strlen(c_name) + strlen(p1_name) + 4;
    }
    if (get_stop(p1_name, head_stop) == NULL) {
        printf("%s: no such stop.\n", p1_name);
        return;
    }
    p1 = get_stop(p1_name, head_stop);

    if (str[i - 1] == '"') {
        strtok(NULL, "\"");
        p2_name = strtok(NULL, "\"");
    } else
        p2_name = strtok(NULL, DELIMITER);
    if (get_stop(p2_name, head_stop) == NULL) {
        printf("%s: no such stop.\n", p2_name);
        return;
    }
    p2 = get_stop(p2_name, head_stop);

    cost = strtok(NULL, DELIMITER);

    dur = strtok(NULL, DELIMITER);

    if (atof(cost) < 0 || atof(dur) < 0) {
        printf("negative cost or duration.\n");
        return;
    }
    if (c->n_stops) {
        if (c->route_cons[c->n_stops - 2]->p2 == p1) {
            is_con0 = 0;
            c->route_cons =
                (con **)realloc(c->route_cons, c->n_stops * sizeof(con *));
            if (c->route_cons == NULL)
                shut_down();
            c->n_stops++;
            for (i = 0; i < p2->n_routes; i++)
                if (p2->routes[i] == c) {
                    p2->n_routes--;
                    break;
                }
            p2->n_routes++;
        } else if (c->route_cons[0]->p1 == p2) {
            is_con0 = 1;
            c->route_cons =
                (con **)realloc(c->route_cons, c->n_stops * sizeof(con *));
            if (c->route_cons == NULL)
                shut_down();
            for (i = c->n_stops - 1; i > 0; i--)
                c->route_cons[i] = c->route_cons[i - 1];
            c->n_stops++;
            for (i = 0; i < p1->n_routes; i++)
                if (p1->routes[i] == c) {
                    p1->n_routes--;
                    break;
                }
            p1->n_routes++;
        } else {
            printf("link cannot be associated with bus line.\n");
            return;
        }
    } else {
        is_con0 = 1;
        if (p1 == p2)
            p1->n_routes++;
        else {
            p1->n_routes++;
            p2->n_routes++;
        }
        c->n_stops = 2;
        c->route_cons = (con **)malloc(sizeof(con *));
        if (c->route_cons == NULL)
            shut_down();
    }

    c->cost += atof(cost);
    c->duration += atof(dur);

    link = malloc(sizeof(con));
    if (link == NULL)
        shut_down();

    if (is_con0) {
        link->r = c;
        link->p1 = p1;
        link->p2 = p2;
        link->cost = atof(cost);
        link->duration = atof(dur);
        c->route_cons[0] = link;
    } else {
        link->r = c;
        link->p1 = p1;
        link->p2 = p2;
        link->cost = atof(cost);
        link->duration = atof(dur);
        c->route_cons[c->n_stops - 2] = link;
    }

    stop_routes(head_route, p1);
    stop_routes(head_route, p2);
}

void intersc(stop *head_stop) {
    int j, n, min;
    stop *stop_ptr;
    char **routes;
    char *temp;

    stop_ptr = head_stop;
    while (stop_ptr != NULL) {
        if (stop_ptr->n_routes <= 1) {
            stop_ptr = stop_ptr->link;
            continue;
        }
        /* routes = malloc(stop_ptr->n_routes * sizeof(route)); */
        routes = malloc(stop_ptr->n_routes * sizeof(char*));
        for (j = 0; j < stop_ptr->n_routes; j++)
            routes[j] = (stop_ptr->routes[j]->name);
        for (j = 0; j < stop_ptr->n_routes; j++) {
            min = j;
            for (n = j + 1; n < stop_ptr->n_routes; n++)
                if (strcmp(routes[n], routes[min]) < 0)
                    min = n;
            if (min != j) {
                temp = routes[j];
                routes[j] = routes[min];
                routes[min] = temp;
            }
        }
        printf("%s %d:", stop_ptr->name, stop_ptr->n_routes);
        for (j = 0; j < stop_ptr->n_routes; j++) {
            printf(" %s", routes[j]);
        }
        printf("\n");
        stop_ptr = stop_ptr->link;
        free(routes);
    }
}

void stop_routes(route *head_route, stop *s) {
    route *route_ptr, **routes;
    int i, acum = 0;
    route_ptr = head_route;

    if (s->routes != NULL)
        free(s->routes);

    routes = malloc(sizeof(route *));
    if (routes == NULL)
        shut_down();
    if (s->n_routes != 0) {
        while (route_ptr != NULL) {
            for (i = 0; i < route_ptr->n_stops - 1; i++) {
                if (s == route_ptr->route_cons[i]->p1 ||
                    s == route_ptr->route_cons[i]->p2) {
                    acum++;
                    routes = (route **)realloc(routes, acum * sizeof(route *));
                    if (routes == NULL)
                        shut_down();
                    routes[acum - 1] = route_ptr;
                    break;
                }
            }
            route_ptr = route_ptr->link;
        }
        s->routes = routes;
    } else
        free(routes);
}

void delete_stop(char *name, stop **head_stop) {
    int i, aux;
    stop *p, *stop_ptr;

    if (get_stop(name, *head_stop) == NULL) {
        printf("%s: no such stop.\n", name);
        return;
    }
    p = get_stop(name, *head_stop);
    stop_ptr = *head_stop;

    if (p) {
        if (p->n_routes) {
            aux = p->n_routes;
            for (i = 0; i < aux; i++) {
                delete_stop_connections(p, p->routes[i]);
            }
            free(p->routes);
        }

        if (p == *head_stop)
            *head_stop = (*head_stop)->link;

        else {
            while (stop_ptr->link != p)
                stop_ptr = stop_ptr->link;
            stop_ptr->link = p->link;
        }
        free(p->name);
        free(p);
    }
}

void delete_route(char *name, route **head_route, stop *head_stop) {
    route *c, *route_ptr;
    stop *stop_ptr;
    int i, j;

    if (get_route(name, *head_route) == NULL) {
        printf("%s: no such line.\n", name);
        return;
    }
    c = get_route(name, *head_route);
    route_ptr = *head_route;

    stop_ptr = head_stop;

    if (c) {
        if (c->n_stops) {
            delete_route_cons(c);
            while (stop_ptr) {
                for (i = 0; i < stop_ptr->n_routes; i++) {
                    if (stop_ptr->routes[i] == c) {
                        for (j = i; j < stop_ptr->n_routes - 1; j++)
                            stop_ptr->routes[j] = stop_ptr->routes[j + 1];
                        stop_ptr->routes =
                            realloc(stop_ptr->routes,
                                    (stop_ptr->n_routes - 1) * sizeof(route *));
                        stop_ptr->n_routes--;
                        break;
                    }
                }
                stop_ptr = stop_ptr->link;
            }
        }

        if (c == *head_route)
            *head_route = (*head_route)->link;

        else {
            while (route_ptr->link != c)
                route_ptr = route_ptr->link;
            route_ptr->link = c->link;
        }
        free(c->name);
        free(c);
    }
}

void kill(stop **head_stop, route **head_route) {
    stop *stop_ptr, *aux_stop;
    route *route_ptr, *aux_route;

    route_ptr = *head_route;
    stop_ptr = *head_stop;

    while (route_ptr) {
        aux_route = route_ptr->link;
        delete_route_cons(route_ptr);
        free(route_ptr->name);
        free(route_ptr);
        route_ptr = aux_route;
    }

    while (stop_ptr) {
        aux_stop = stop_ptr->link;
        if (stop_ptr->routes)
            free(stop_ptr->routes);
        free(stop_ptr->name);
        free(stop_ptr);
        stop_ptr = aux_stop;
    }

    *head_stop = NULL;
    *head_route = NULL;
}

void destino(char *name, stop *head_stop, route *head_route){
    int has_space = 0;
    route *route_ptr;

    route_ptr = head_route;
    if(get_stop(name, head_stop) == NULL)
        printf("%s: no such stop.\n", name);
    else{
        while (route_ptr){
            if (route_ptr->n_stops != 0)
                if (route_ptr->route_cons[route_ptr->n_stops - 2]->p2 == get_stop(name, head_stop)){
                    if (has_space)
                        printf(" %s", route_ptr->name);
                    else{
                        printf("%s", route_ptr->name);
                        has_space = 1;
                    }
                }
            route_ptr = route_ptr->link;
        }
        if (has_space)
            printf("\n");
    }
}
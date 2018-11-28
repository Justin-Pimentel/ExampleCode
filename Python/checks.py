#Credit to Shuo-En Li we wrote this as partners

def if_neutral_planet_available(state):
    return any(state.neutral_planets()) #81:19 aggressive bot #83:17 spread bot #81:19 production bot #91:9 defense bot #95:5 ez bot


def have_largest_fleet(state):
    # return sum(planet.num_ships for planet in state.my_planets()) \
    #          + sum(fleet.num_ships for fleet in state.my_fleets()) \
    #        > sum(planet.num_ships for planet in state.enemy_planets()) \
    #          + sum(fleet.num_ships for fleet in state.enemy_fleets())
        return sum(planet.num_ships for planet in state.my_planets()) \
           > sum(planet.num_ships for planet in state.enemy_planets()) \
             

def have_weak_planets(state):
    my_planets = [planet for planet in state.my_planets()
                      if not any(fleet.destination_planet == planet.ID for fleet in state.my_fleets())]
    my_planets.sort(key=lambda p: p.num_ships)
    if not my_planets:
        return False
    avg_ships =  sum(planet.num_ships for planet in state.my_planets()) / len(state.my_planets())
    upper = avg_ships + my_planets[0].num_ships
    lower = avg_ships - my_planets[0].num_ships
    if lower < my_planets[0].num_ships < upper:
        return True
    else:
        return False

def planet_under_attack(state):

    for fleet in state.enemy_fleets():
        if fleet.destination_planet in state.my_planets():
            target_planet = state.planets[fleet.destination_planet]
            if fleet.num_ships > target_planet.num_ships:
                return True
    return False

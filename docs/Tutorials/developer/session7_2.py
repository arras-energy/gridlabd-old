# session7_2.py

import sys, math, datetime

real_power = {}
power_factor = {}
power = {}

def on_precommit(t):
    print(f"\nEVENT: on_precommit(t={t})",file=sys.stderr)
    print(f"\n*** advancing clock to {datetime.datetime.fromtimestamp(t)} ***",file=sys.stderr)
    return gridlabd.NEVER

#
# Class event handlers
#

def example_init(obj,t):
    print(f"\nEVENT: {obj}.on_init(t={t})",file=sys.stderr)

    # create the accessors
    power[obj] = gridlabd.property(obj,'power')
    real_power[obj] = gridlabd.property(obj,'real_power')
    power_factor[obj] = gridlabd.property(obj,'power_factor')

    # update the power values
    update_power(obj)

    return False # !!!

def example_commit(obj,t):
    print(f"\nEVENT: {obj}.on_commit(t={t})",file=sys.stderr)
    print(f"       {obj}.power = {power[obj]}",file=sys.stderr)
    return gridlabd.NEVER

#
# Object event handlers
#

def main_presync(obj,t):
    print(f"\nEVENT: {obj}.on_presync(t={t})",file=sys.stderr)

    # reset the main accumulator
    power[obj].set_value(0j)
    print(f"       {obj}.power = {power[obj]}",file=sys.stderr)

    return gridlabd.NEVER

def main_postsync(obj,t):
    print(f"\nEVENT: {obj}.on_postsync(t={t})",file=sys.stderr)

    # update the main real power and power factor
    S = power[obj].get_value()
    P = S.real
    PF = P/abs(S)
    gridlabd.set_value(obj,'real_power',str(P))
    gridlabd.set_value(obj,'power_factor',str(PF))

    print(f"       {obj}.power = {power[obj]}",file=sys.stderr)
    print(f"       {obj}.real_power = {real_power[obj]}",file=sys.stderr)
    print(f"       {obj}.power_factor] = {power_factor[obj]}",file=sys.stderr)
    return gridlabd.NEVER

def sub_sync(obj,t):
    print(f"\nEVENT: {obj}.on_sync(t={t})",file=sys.stderr)

    # update local power values
    update_power(obj)

    # update main
    main_power = power['main'].get_value()
    power['main'].set_value(main_power + power[obj].get_value())

    print(f"       main.power = {power['main']}",file=sys.stderr)
    return gridlabd.NEVER

#
# Update utility
#

def update_power(obj):
    global properties

    # read the inputs
    P = real_power[obj].get_value()
    PF = power_factor[obj].get_value()
    
    # calculate the output
    Q = P * math.tan(math.acos(PF))
    S = P + 1j*Q
    
    # only update changed outputs
    if power[obj].get_value() != S:
        power[obj].set_value(S)
        print(f'       {obj}.power: P={P:.6f}, PF={PF:.2f} -> S={S:.6f}',file=sys.stderr)
    else:
        print(f'       {obj}.power: S not changed',file=sys.stderr)


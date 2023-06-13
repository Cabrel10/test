import sys
import os
import signal
import time
from ss7.tracking import *
from ss7.interception import *
from ss7.fraud import *
from ss7.dos import *
from ss7main import *


def banner(word):
    letterforms =  '''\
       |       |       |       |       |       |       | |

 XX    |X  X  X|    XX |       |       |       |       |~|
'''.splitlines()

    table = {}
    for form in letterforms:
        if '|' in form:
            table[form[-2]] = form[:-3].split('|')

    ROWS = len(list(table.values())[0])

    for row in range(ROWS):
        for c in word:
            print (table[c][row],)
        print()
    print()

def mainMenu():
    os.system('clear')
    banner('SiGploit')
    print("\033[33m[-][-]\033[0m\t\tCadre d'exploitation de signalisation\t\033[33m [-][-]\033[0m")
    print("\033[33m[-][-]\033[0m\t\t\tVersion:\033[31mBETA 0.4\033[0m\t\t\033[33m [-][-]\033[0m")
    print("\033[33m[-][-]\033[0m\t\tAuteur:\033[32mLoay AbdelRazek(@sigploit)\033[0m\t\033[33m [-][-]\033[0m\n")
    print()
    print("Contributeurs:")

    print("\t\033[31mRosalia D'Alessandro - TelecomItalia\033[0m")
    print()
    print()
    print()
    print()
    print()
    print("   Module".rjust(10) + "\t\t\tDescription")
    print("   --------                --------------------")
    print("0) SS7".rjust(8) + "\t\tAttaques vocales et SMS 2G/3G")
    print("1) Diameter".rjust(13) + "\t\tAttaques de données 4G")
    print("2) GTP".rjust(8) + "\t\tAttaques de données 3G")
    print("3) SIP".rjust(8) + "\t\tAttaques VoLTE 4G")

    print()
    print("ou quitter pour quitter SiGploit\n".rjust(28))

    choix = input("\033[34msig\033[0m\033[37m>\033[0m ")

    if choix == "0":
        os.system('clear')
        ss7main.attacksMenu()

    elif choix == "1":
        print("\n\033[34m[*]\033[0mDiameter sera mis à jour dans la version 2...")
        print("\033[34m[*]\033[0mRetour au menu principal")
        time.sleep(2)
        mainMenu()

    elif choix == "2":
        print("\n\033[34m[*]\033[0mGTP sera mis à jour dans la version 3...")
        print("\033[34m[*]\033[0mRetour au menu principal")
        time.sleep(2)
        mainMenu()

    elif choix == "3":
        print("\n\033[34m[*]\033[0mSIP sera mis à jour dans la version 4...")
        print("\033[34m[*]\033[0mRetour au menu principal")
        time.sleep(2)
        mainMenu()

    elif choix == "quitter" or choix == "exit":
        print('\nVous quittez maintenant SiGploit...')
        time.sleep(1)
        sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)

if __name__ == '__main__':
    mainMenu()

if __name__ == '__sigploit__':
    LocationTracking()
    Interception()
    Fraud()
    DoS()
    attacksMenu()
    mainMenu()


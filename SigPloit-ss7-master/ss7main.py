#!/usr/bin/env python
# encoding: utf-8
'''
SS7 main 

@author:     Pepit45K

@copyright: 2023. All rights reserved.

@license:    MIT license cewl
'''
import os
import time
import ss7.tracking
import ss7.fraud
import ss7.interception
import ss7.dos
import sigploit


def LocationTracking():
    os.system('clear')

    print(" \033[31mLocalisation\033[0m ".center(105, "#"))
    print(" \033[34mSélectionnez un message dans la liste ci-dessous\033[0m ".center(105, "#"))
    print()
    print ("   Identifiant du Message".rjust(10) + "\t\t\t Catégorie")
    print ("   ---------------------".rjust(10) + "\t\t\t ----------")
    print ("0) SendRoutingInfo".rjust(21) + "\t\t CAT1")
    print ("1) ProvideSubsriberInfo".rjust(26) + "\t CAT2")
    print ("2) SendRoutingInfoForSM".rjust(26) + "\t CAT3")
    print ("3) AnyTimeInterrogation".rjust(26) + "\t CAT1")
    print ("4) SendRoutingInfoForGPRS".rjust(28) + "\t CAT1")

    print("")
    print ("ou tapez 'retour' pour retourner au menu des attaques".rjust(80))

    choix = input(
        "\033[37m(\033[0m\033[2;31mLocalisation\033[0m\033[37m)>\033[0m ")

    if choix == "0":
        ss7.tracking.sri()
    elif choix == "1":
        ss7.tracking.psi()
    elif choix == "2":
        ss7.tracking.srism()
    elif choix == "3":
        ss7.tracking.ati()
    elif choix == "4":
        ss7.tracking.srigprs()
    elif choix == "retour":
        attacksMenu()
    else:
        print ('\n\033[31m[-]Erreur:\033[0m Veuillez entrer un choix valide (0 - 4)')
        time.sleep(1.5)
        LocationTracking()


def Interception():
    os.system('clear')

    print(" \033[31mInterception\033[0m ".center(105, "#"))
    print(" \033[34mSélectionnez un message dans la liste ci-dessous\033[0m ".center(105, "#"))
    print("")
    print ("   Identifiant du Message".rjust(10) + "\t\t\t\t Catégorie")
    print ("   ---------------------".rjust(10) + "\t\t\t\t ----------")
    print ("0) UpdateLocation-SMS Interception".rjust(37) + "\t CAT3")

    print("")
    print ("ou tapez 'retour' pour retourner au menu des attaques".rjust(80))

    choix = input(
        "\033[37m(\033[0m\033[2;31mInterception\033[0m\033[37m)>\033[0m ")

    if choix == "0":
        ss7.interception.ul()

    elif choix == "retour":
        attacksMenu()
    else:
        print ('\n\033[31m[-]Erreur:\033[0m Veuillez entrer un choix valide (0)')
        time.sleep(1.5)
        Interception()


def Fraud():
    os.system('clear')

    print (" \033[31mFraude\033[0m ".center(105, "#"))
    print (" \033[34mSélectionnez un message dans la liste ci-dessous\033[0m ".center(105, "#"))
    print("")
    print ("   Identifiant du Message".rjust(10) + "\t\t\t\t Catégorie")
    print ("   ---------------------".rjust(10) + "\t\t\t\t ----------")
    print ("0) SendIMSI".rjust(14) + "\t\t\t\t CAT3")
    print ("1) MTForwardSMS SMS Spoofing".rjust(31) + "\t CAT3")

    print("")
    print ("ou tapez 'retour' pour retourner au menu des attaques".rjust(80))

    choix = input(
        "\033[37m(\033[0m\033[2;31mFraude\033[0m\033[37m)>\033[0m ")

    if choix == "0":
        ss7.fraud.simsi()
    elif choix == "1":
        ss7.fraud.mtsms()


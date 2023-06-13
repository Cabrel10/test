#!/usr/bin/env python
'''
Created on jun 2023

@author: 45K
'''

import sys
import os
import time
import sigploit
import ss7main

from subprocess import *


ul_path = os.path.join(os.getcwd(),'ss7/attacks/interception/ul')



def ul():
	
	jar_file = 'UpdateLocation.jar'

	try:
		updateLocation = check_call(['java','-jar', os.path.join(ul_path,jar_file)])
		if updateLocation == 0:
			it = raw_input('\nVoulez-vous retourner au menu d\'interception ? (o/n): ')
			if it == 'o' or it == 'oui':
				ss7main.Interception()
			elif it == 'n' or it == 'non':
				attack_menu = raw_input('Voulez-vous choisir une autre catégorie d\'attaques ? (o/n): ')
				if attack_menu == 'o' or attack_menu =='oui':
					ss7main.attacksMenu()
				elif attack_menu == 'n' or attack_menu =='non':
					main_menu = raw_input('Souhaitez-vous revenir au menu principal ? (o/quitter): ')
					if main_menu == 'o' or main_menu =='oui':
						sigploit.mainMenu()
					elif main_menu =='quitter':
						print ('Fin du programme...')
						time.sleep(1)
						sys.exit(0)
			
	
	except CalledProcessError as e:
		print ("\033[31m[-]Erreur:\033[0m Erreur lors du lancement d'UpdateLocation, erreur : " + e.message)
		time.sleep(2)
		ss7main.Interception()


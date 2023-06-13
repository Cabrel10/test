#!/usr/bin/env python

'''
Created on June 2023

@author: 45K
'''
import sys
import os
import time
import sigploit
import ss7main
from subprocess import *



purge_path = os.path.join(os.getcwd(),'ss7/attacks/dos/prgms')



def purge():
	
	jar_file = 'PurgeMS.jar'

	try:
		purgeMS = check_call(['java','-jar', os.path.join(purge_path,jar_file)])
		if purgeMS == 0:
			ds = raw_input('\nVoulez-vous retourner au menu "DoS" ? (o/n): ')
			if ds == 'o' or ds == 'oui':
				ss7main.DoS()
			elif ds == 'n' or ds == 'non':
				attack_menu = raw_input('Souhaitez-vous choisir une autre catégorie d\'attaques ? (o/n): ')
				if attack_menu == 'o'or attack_menu =='oui':
					ss7main.attacksMenu()
				elif attack_menu == 'n' or attack_menu =='non':
					main_menu = raw_input('Souhaitez-vous revenir au menu principal ? (o/quitter): ')
					if main_menu == 'o' or main_menu =='oui':
						sigploit.mainMenu()
					elif main_menu =='quitter':
						print ('Fin du programme...')
						sys.exit(0)
			
	
	except CalledProcessError as e:
		print ("\033[31m[-]\033[0mErreur: Impossible de lancer PurgeMS, " + str(e))
		time.sleep(2)
		ss7main.DoS()

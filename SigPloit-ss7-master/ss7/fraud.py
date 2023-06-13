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


simsi_path = os.path.join(os.getcwd(),'ss7/attacks/fraud/simsi')
mtsms_path = os.path.join(os.getcwd(),'ss7/attacks/fraud/mtsms')
cl_path = os.path.join(os.getcwd(),'ss7/attacks/fraud/cl')

def simsi():
	
	jar_file = 'SendIMSI.jar'

	try:
		sendIMSI = check_call(['java','-jar', os.path.join(simsi_path,jar_file)])
		if sendIMSI == 0:
			fr = raw_input('\nVoulez-vous retourner au menu "Fraud" ? (o/n): ')
			if fr == 'o' or fr == 'oui':
				ss7main.Fraud()
			elif fr == 'n' or fr == 'non':
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
		print ("\033[31m[-]Erreur:\033[0m Lancement de SendIMSI impossible, " + str(e))
		time.sleep(2)
		ss7main.Fraud()


def mtsms():
	
	jar_file = 'MTForwardSMS.jar'

	try:
		mtForwardSMS = check_call(['java','-jar', os.path.join(mtsms_path,jar_file)])
		if mtForwardSMS == 0:
			fr = raw_input('\nVoulez-vous retourner au menu "Fraud" ? (o/n): ')
			if fr == 'o' or fr == 'oui':
				ss7main.Fraud()
			elif fr == 'n' or fr == 'non':
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
		print ("\033[31m[-]Erreur:\033[0m Lancement de MTForwardSMS impossible, " + str(e))
		time.sleep(2)
		ss7main.Fraud()
	

def cl():
	
	jar_file = 'CancelLocation.jar'

	try:
		cancelLocation = check_call(['java','-jar', os.path.join(cl_path,jar_file)])
		if cancelLocation == 0:
			fr = raw_input('\nVoulez-vous retourner au menu "Fraud" ? (o/n): ')
			if fr == 'o' or fr == 'oui':
				ss7main.Fraud()
			elif fr == 'n' or fr == 'non':
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
		print ("\033[31m[-]Erreur:\033[0m Lancement de CancelLocation impossible, " + str(e))
		time.sleep(2)
		ss7main.Fraud()

#!/usr/bin/env python
'''
Created on 1 Feb 2018

@author: loay
'''

import sys
import os
import time
import sigploit
import ss7main

from subprocess import *


sms_privacy_path = os.path.join(os.getcwd(),'ss7/attacks/sms_privacy')
sms_spam_path = os.path.join(os.getcwd(),'ss7/attacks/sms_spam')
call_flooding_path = os.path.join(os.getcwd(),'ss7/attacks/call_flooding')
location_tracking_path = os.path.join(os.getcwd(),'ss7/attacks/location_tracking')
imsi_catcher_path = os.path.join(os.getcwd(),'ss7/attacks/IMSI_Catcher')


def smsSpoof():
	jar_file = 'SendAttackerSMS.jar'

	try:
		sms_spoof = check_call(['java','-jar', os.path.join(sms_privacy_path,jar_file)])
		if sms_spoof == 0:
			sp_media_menu = raw_input('\nVoulez-vous continuer avec une autre attaque de la catégorie "SMS Privacy" ? (o/n): ')
			if sp_media_menu == 'o' or sp_media_menu == 'oui':
				ss7main.smsPrivacy()
			elif sp_media_menu == 'n' or sp_media_menu == 'non':
				attack_menu = raw_input('Voulez-vous choisir une autre catégorie d\'attaques ? (o/n): ')
				if attack_menu == 'o' or attack_menu == 'oui':
					ss7main.attacksMenu()
				elif attack_menu == 'n' or attack_menu == 'non':
					main_menu = raw_input('Souhaitez-vous retourner au menu principal ? (o/quitter): ')
					if main_menu == 'o' or main_menu == 'oui':
						sigploit.mainMenu()
					elif main_menu == 'quitter':
						print ('Fin du programme...')
						sys.exit(0)

	except CalledProcessError as e:
		print ("\033[31m[-]Erreur:\033[0m"+jar_file+" n'a pas pu être lancé, " + e.message)
		time.sleep(2)
		ss7main.smsPrivacy()

def smsSpam():
	jar_file = 'SendSMSAttack.jar'

	try:
		sms_spamming = check_call(['java','-jar', os.path.join(sms_spam_path,jar_file)])
		if sms_spamming == 0:
			sp_media_menu = raw_input('\nVoulez-vous continuer avec une autre attaque de la catégorie "SMS Spam" ? (o/n): ')
			if sp_media_menu == 'o' or sp_media_menu == 'oui':
				ss7main.smsSpam()
			elif sp_media_menu == 'n' or sp_media_menu == 'non':
				attack_menu = raw_input('Voulez-vous choisir une autre catégorie d\'attaques ? (o/n): ')
				if attack_menu == 'o' or attack_menu == 'oui':
					ss7main.attacksMenu()
				elif attack_menu == 'n' or attack_menu == 'non':
					main_menu = raw_input('Souhaitez-vous retourner au menu principal ? (o/quitter): ')
					if main_menu == 'o' or main_menu == 'oui':
						sigploit.mainMenu()
					elif main_menu == 'quitter':
						print ('Fin du programme...')
						sys.exit(0)

	except CalledProcessError as e:
		print ("\033[31m[-]Erreur:\033[0m"+jar_file+" n'a pas pu être lancé, " + e.message)
		time.sleep(2)
		ss7main.smsSpam()

def callFlooding():
	jar_file = 'Call_Flooder.jar'

	try:
		call_flooding = check_call(['java','-jar', os.path.join(call_flooding_path,jar_file)])
		if call_flooding == 0:
			cf_media_menu = raw_input('\nVoulez-vous continuer avec une autre attaque de la catégorie "Call Flooding" ? (o/n): ')
			if cf_media_menu == 'o' or cf_media_menu == 'oui':
				ss7main.callFlooding()
			elif cf_media_menu == 'n' or cf_media_menu == 'non':
				attack_menu = raw_input('Voulez-vous choisir une autre catégorie d\'attaques ? (o/n): ')
				if attack_menu == 'o' or attack_menu == 'oui':
					ss7main.attacksMenu()
				elif attack_menu == 'n' or attack_menu == 'non':
					main_menu = raw_input('Souhaitez-vous retourner au menu principal ? (o/quitter): ')
					if main_menu == 'o' or main_menu == 'oui':
						sigploit.mainMenu()
					elif main_menu == 'quitter':
						print ('Fin du programme...')
						sys.exit(0)

	except CalledProcessError as e:
		print ("\033[31m[-]Erreur:\033[0m"+jar_file+" n'a pas pu être lancé, " + e.message)
		time.sleep(2)
		ss7main.callFlooding()


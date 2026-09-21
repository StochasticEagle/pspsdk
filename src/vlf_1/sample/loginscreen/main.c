#include <pspsdk.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspsuspend.h>
#include <psppower.h>
#include <pspreg.h>
#include <psprtc.h>
#include <psputils.h>
#include <pspgu.h>
#include <pspgum.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>

#include <vlf.h>

PSP_MODULE_INFO("VlfSample", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(0);

typedef struct _SampleConfiguration
{
	u16 username[32];
	u16 password[32];
} SampleConfiguration;

char *license_text =
		"BLAHBLAHBLAH LICENSE.\n"
		" Version 1, 11 january 2009\n\n"
		"This is an imaginary sample license.\n"
		"It is here just to show a scrollbar.\n\n"
		"BLAHHHHHHHHHHHHHHHHHHHH\n"
		"BLAHHHHHHHHHHHHHHHHHHHH\n"
		"BLAHHHHHHHHHHHHHHHHHHHH\n"
		"BLAHHHHHHHHHHHHHHHHHHHH\n"
		"BLAHHHHHHHHHHHHHHHHHHHH\n"
		"BLAHHHHHHHHHHHHHHHHHHHH\n"
		"BLAHHHHHHHHHHHHHHHHHHHH\n"
		"BLAHHHHHHHHHHHHHHHHHHHH\n"
		"BLAHHHHHHHHHHHHHHHHHHHH\n"		
		"BLAHHHHHHHHHHHHHHHHHHHH\n"
		"BLAHHHHHHHHHHHHHHHHHHHH";
		
SampleConfiguration config;
VlfPicture logo;		
		
VlfText license, do_you_agree;

int focus;
VlfText username_label, password_label, rp_label, signin;
VlfInputBox username, password;
VlfCheckBox rp;

int OnExit(int enter)
{
	if (enter)
		sceKernelExitGame();
	
	return VLF_EV_RET_NOTHING;
}

void OnSaveConfig(void *param)
{
	int savepassword = (int)param;
	
	if (!savepassword)
		memset(config.password, 0, 32);
	
	SceUID fd = sceIoOpen("config.bin", PSP_O_WRONLY | PSP_O_TRUNC | PSP_O_CREAT, 0777);
	sceIoWrite(fd, &config, sizeof(SampleConfiguration));
	sceIoClose(fd);
}

void OnLoginFadeOutComplete(void *param)
{
	// Destruct old items
	vlfGuiCancelBottomDialog();
	vlfGuiRemoveText(username_label);
	vlfGuiRemoveText(password_label);
	vlfGuiRemoveText(rp_label);
	vlfGuiRemoveText(signin);
	vlfGuiRemoveInputBox(username);
	vlfGuiRemoveInputBox(password);
	vlfGuiRemoveCheckBox(rp);
	
	// Construct new items
	VlfText done = vlfGuiAddText(240, 110, "Sign in successfull!\nConfiguration saved.");
	vlfGuiSetTextAlignment(done, VLF_ALIGNMENT_CENTER);	
	vlfGuiCustomBottomDialog(NULL, "Exit", 1, 0, VLF_DEFAULT, OnExit);	
	
	vlfGuiSetRectangleFade(0, VLF_TITLEBAR_HEIGHT, 480, 272-VLF_TITLEBAR_HEIGHT, VLF_FADE_MODE_IN, VLF_FADE_SPEED_SUPER_FAST, 0, OnSaveConfig, param, 0);
}

int OnLoginScreenUp(void *param)
{
	switch (focus)
	{
		case 1:
			focus = 0;
			vlfGuiSetInputBoxFocus(password, 0);
			vlfGuiSetInputBoxFocus(username, 1);
			break;
		
		case 2:
			focus = 1;
			vlfGuiSetCheckBoxFocus(rp, 0);
			vlfGuiSetInputBoxFocus(password, 1);
			break;
		
		case 3:
			focus = 2;
			vlfGuiRemoveTextFocus(signin, 1);
			vlfGuiSetCheckBoxFocus(rp, 1);
			break;
	}
	
	return VLF_EV_RET_NOTHING;
}

int OnLoginScreenDown(void *param)
{
	switch (focus)
	{
		case 0:
			focus = 1;
			vlfGuiSetInputBoxFocus(username, 0);
			vlfGuiSetInputBoxFocus(password, 1);
			break;
		
		case 1:
			focus = 2;
			vlfGuiSetInputBoxFocus(password, 0);
			vlfGuiSetCheckBoxFocus(rp, 1);
			break;
		
		case 2:
			focus = 3;
			vlfGuiSetCheckBoxFocus(rp, 0);
			vlfGuiSetTextFocus(signin);
			break;
	}
	
	return VLF_EV_RET_NOTHING;
}

int OnLogin(int enter)
{
	if (enter)
	{
		if (focus == 3)
		{			
			vlfGuiGetInputBoxText(username, config.username);
			
			if (config.username[0] == 0)
			{
				vlfGuiMessageDialog("Username is empty!", VLF_MD_TYPE_ERROR | VLF_MD_BUTTONS_NONE);
				return VLF_EV_RET_NOTHING;
			}
			
			vlfGuiGetInputBoxText(password, config.password);
			
			if (config.password[0] == 0)
			{
				vlfGuiMessageDialog("Password is empty!", VLF_MD_TYPE_ERROR | VLF_MD_BUTTONS_NONE);
				return VLF_EV_RET_NOTHING;
			}
			
			vlfGuiRemoveEventHandler(OnLoginScreenUp);
			vlfGuiRemoveEventHandler(OnLoginScreenDown);
			
			vlfGuiSetRectangleFade(0, VLF_TITLEBAR_HEIGHT, 480, 272-VLF_TITLEBAR_HEIGHT, VLF_FADE_MODE_OUT, VLF_FADE_SPEED_SUPER_FAST, 0, OnLoginFadeOutComplete, (void *)vlfGuiIsCheckBoxChecked(rp), 0);
			
			return VLF_EV_RET_REMOVE_HANDLERS;
		}
	}
	else
	{
		sceKernelExitGame();
	}
	
	return VLF_EV_RET_NOTHING;
}

void OnLicenseAgreementFadeOutComplete(void *param)
{
	// Destruct old items	
	vlfGuiRemoveText(license);
	vlfGuiRemoveText(do_you_agree);
	vlfGuiCancelBottomDialog();
	
	// Construct new items
	vlfGuiBottomDialog(VLF_DI_CANCEL, VLF_DI_ENTER, 1, 0, VLF_DEFAULT, OnLogin);
	
	username = vlfGuiAddInputBox("Username", 64, 65);
	password = vlfGuiAddInputBox("Password", 64, 115);
	username_label = vlfGuiAddText(87, 45, "Username");
	password_label = vlfGuiAddText(87, 95, "Password");	
	rp = vlfGuiAddCheckBox(70, 150);
	rp_label = vlfGuiAddText(95, 153, "Remember password");
	signin = vlfGuiAddText(240, 195, "Sign In");
	
	vlfGuiSetTextFontSize(username_label, 0.75f);
	vlfGuiSetTextFontSize(password_label, 0.75f);
	vlfGuiSetTextAlignment(signin, VLF_ALIGNMENT_CENTER);
	vlfGuiSetInputBoxType(password, VLF_INPUTBOX_TYPE_PASSWORD);
	vlfGuiSetInputBoxTextW(username, config.username);
	vlfGuiSetInputBoxTextW(password, config.password);
	
	if (config.username[0] == 0)
	{
		vlfGuiSetInputBoxFocus(password, 0);
		focus = 0;
	}
	else if (config.password[0] == 0)
	{
		vlfGuiSetInputBoxFocus(username, 0);
		focus = 1;
	}
	else
	{
		vlfGuiSetInputBoxFocus(username, 0);
		vlfGuiSetInputBoxFocus(password, 0);
		vlfGuiSetCheckBoxCheck(rp, 1);
		vlfGuiSetTextFocus(signin);
		focus = 3;
	}
	
	vlfGuiAddEventHandler(PSP_CTRL_UP, 0, OnLoginScreenUp, NULL);
	vlfGuiAddEventHandler(PSP_CTRL_DOWN, 0, OnLoginScreenDown, NULL);
	
	vlfGuiSetRectangleFade(0, VLF_TITLEBAR_HEIGHT, 480, 272-VLF_TITLEBAR_HEIGHT, VLF_FADE_MODE_IN, VLF_FADE_SPEED_SUPER_FAST, 0, NULL, NULL, 0);	
}

int OnLicenseAgreement(int enter)
{
	if (enter)
	{
		vlfGuiSetRectangleFade(0, VLF_TITLEBAR_HEIGHT, 480, 272-VLF_TITLEBAR_HEIGHT, VLF_FADE_MODE_OUT, VLF_FADE_SPEED_SUPER_FAST, 0, OnLicenseAgreementFadeOutComplete, NULL, 0);
		return VLF_EV_RET_REMOVE_HANDLERS;
	}
	
	sceKernelExitGame();
	
	return VLF_EV_RET_NOTHING;
}
		
void StartSample()
{
	license = vlfGuiAddText(100, 40, license_text);			
	vlfGuiSetTextScrollBar(license,  90);
	
	do_you_agree = vlfGuiAddText(120, 210, "Do you agree to the terms?");
	vlfGuiBottomDialog(VLF_DI_CANCEL, VLF_DI_ENTER, 1, 0, VLF_DEFAULT, OnLicenseAgreement);
	
	vlfGuiSetRectangleFade(0, VLF_TITLEBAR_HEIGHT, 480, 272-VLF_TITLEBAR_HEIGHT, VLF_FADE_MODE_IN, VLF_FADE_SPEED_SUPER_FAST, 0, NULL, NULL, 0);
}

int menu_sel(int sel)
{
	switch (sel)
	{
		case 0:			
			StartSample();	
			vlfGuiRemovePicture(logo);		
		return VLF_EV_RET_REMOVE_HANDLERS | VLF_EV_RET_REMOVE_OBJECTS;
		
		case 1:
			sceKernelExitGame();
		break;
	}
	
	return VLF_EV_RET_NOTHING;
}

int app_main(int argc, char *argv[])
{
	char *items[] = { "Start sample", "Exit" };
	
	vlfGuiSystemSetup(1, 1, 1);		
	vlfGuiLateralMenu(2, items, 0, menu_sel, 100);
	
	logo = vlfGuiAddPictureFile("logo.png", 95, 80); // png's are now supported
	
	SceUID fd = sceIoOpen("config.bin", PSP_O_RDONLY, 0);
	if (fd >= 0)
	{
		sceIoRead(fd, &config, sizeof(SampleConfiguration));
		sceIoClose(fd);
	}
	
	while (1)
	{
		vlfGuiDrawFrame();
	}
	
	return 0;
}


#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include "phone_common.h"
#include "cfg_xml.h"
#include "lan_strings.h"

FILE *arr_file;
FILE *define_file;
FILE *id_file;
FILE *char_file;

pj_bool_t is_cur_language_show_from_right_to_left(int language_id)
{
	if (language_id == HEBREW
		|| language_id == FARSI)
		return PJ_TRUE;
	else
		return PJ_FALSE;
}


const pj_uint16_t Arbic_Position[][4]=/*last, first, middle, alone*/
{
	{ 0xfe80, 0xfe80, 0xfe80, 0xfe80}, /*0x0621*/
	{ 0xfe82, 0xfe81, 0xfe82, 0xfe81}, /*0x0622*/
	{ 0xfe84, 0xfe83, 0xfe84, 0xfe83}, /*0x0623*/
	{ 0xfe86, 0xfe85, 0xfe86, 0xfe85}, /*0x0624*/
	{ 0xfe88, 0xfe87, 0xfe88, 0xfe87}, /*0x0625*/
	{ 0xfe8a, 0xfe8b, 0xfe8c, 0xfe89}, /*0x0626*/
	{ 0xfe8e, 0xfe8d, 0xfe8e, 0xfe8d}, /*0x0627*/
	{ 0xfe90, 0xfe91, 0xfe92, 0xfe8f}, /*0x0628*/
	{ 0xfe94, 0xfe93, 0xfe93, 0xfe93}, /*0x0629*/
	{ 0xfe96, 0xfe97, 0xfe98, 0xfe95}, /*0x062A*/
	{ 0xfe9a, 0xfe9b, 0xfe9c, 0xfe99}, /*0x062B*/
	{ 0xfe9e, 0xfe9f, 0xfea0, 0xfe9d}, /*0x062C*/
	{ 0xfea2, 0xfea3, 0xfea4, 0xfea1}, /*0x062D*/
	{ 0xfea6, 0xfea7, 0xfea8, 0xfea5}, /*0x062E*/
	{ 0xfeaa, 0xfea9, 0xfeaa, 0xfea9}, /*0x062F*/
	{ 0xfeac, 0xfeab, 0xfeac, 0xfeab}, /*0x0630*/
	{ 0xfeae, 0xfead, 0xfeae, 0xfead}, /*0x0631*/
	{ 0xfeb0, 0xfeaf, 0xfeb0, 0xfeaf}, /*0x0632*/
	{ 0xfeb2, 0xfeb3, 0xfeb4, 0xfeb1}, /*0x0633*/
	{ 0xfeb6, 0xfeb7, 0xfeb8, 0xfeb5}, /*0x0634*/
	{ 0xfeba, 0xfebb, 0xfebc, 0xfeb9}, /*0x0635*/
	{ 0xfebe, 0xfebf, 0xfec0, 0xfebd}, /*0x0636*/
	{ 0xfec2, 0xfec3, 0xfec4, 0xfec1}, /*0x0637*/
	{ 0xfec6, 0xfec7, 0xfec8, 0xfec5}, /*0x0638*/
	{ 0xfeca, 0xfecb, 0xfecc, 0xfec9}, /*0x0639*/
	{ 0xfece, 0xfecf, 0xfed0, 0xfecd}, /*0x063A*/
	{ 0x63b, 0x63b, 0x63b, 0x63b}, /*0x063B*/
	{ 0x63c, 0x63c, 0x63c, 0x63c},  /*0x063C*/
	{ 0x63d, 0x63d, 0x63d, 0x63d}, /*0x063D*/
	{ 0x63e, 0x63e, 0x63e, 0x63e}, /*0x063E*/
	{ 0x63f, 0x63f, 0x63f, 0x63f},   /*0x063F*/
	{ 0x640, 0x640, 0x640, 0x640}, /*0x640*/  /*'-'*/ 
	{ 0xfed2, 0xfed3, 0xfed4, 0xfed1}, /*0x641*/
	{ 0xfed6, 0xfed7, 0xfed8, 0xfed5}, /*0x642*/
	{ 0xfeda, 0xfedb, 0xfedc, 0xfed9}, /*0x643*/
	{ 0xfede, 0xfedf, 0xfee0, 0xfedd}, /*0x644*/
	{ 0xfee2, 0xfee3, 0xfee4, 0xfee1}, /*0x645*/
	{ 0xfee6, 0xfee7, 0xfee8, 0xfee5}, /*0x646*/
	{ 0xfeea, 0xfeeb, 0xfeec, 0xfee9}, /*0x647*/
	{ 0xfeee, 0xfeed, 0xfeee, 0xfeed}, /*0x648*/
	{ 0xfef0, 0xfeef, 0xfef0, 0xfeef},  /*0x649*/
	{0xfef2, 0xfef3, 0xfef4, 0xfef1},  /*0x64a*/
};

const pj_uint16_t Persian_Position[][4]=/*last, first, middle, alone*/
{
	{0xfb57, 0xfb58, 0xfb59, 0xfb56},   // 0x067e
	{0xfb7b, 0xfb7c, 0xfb7d, 0xfb7a},   // 0x0686
	{0xfb8b, 0xfb8a, 0xfb8b, 0xfb8a},   // 0x0698
	{0xfb8f, 0xfb90, 0xfb91, 0xfb8e},   // 0x06a9
	{0xfb93, 0xfb94, 0xfb95, 0xfb92},   // 0x06af
	{0xfbfd, 0xfbfe, 0xfbff, 0xfbfc},   // 0x06cc
};

static pj_uint16_t arabic_specs[][2]=
{
	{0xFEF5, 0xFEF6},
	{0xFEF7, 0xFEF8},
	{0xFEF9, 0xFEFA},
	{0xFEFB, 0xFEFC},
};

#if 0
static pj_uint16_t theSet1[23]={
	0x62c, 0x62d, 0x62e, 0x647, 0x639, 0x63a, 0x641, 0x642, 
	0x62b, 0x635, 0x636, 0x637, 0x643, 0x645, 0x646, 0x62a, 
	0x644, 0x628, 0x64a, 0x633, 0x634, 0x638, 0x626
};


static pj_uint16_t theSet2[35]={
	0x62c, 0x62d, 0x62e, 0x647, 0x639, 0x63a, 0x641, 0x642, 
	0x62b, 0x635, 0x636, 0x637, 0x643, 0x645, 0x646, 0x62a, 
	0x644, 0x628, 0x64a, 0x633, 0x634, 0x638, 0x626, 
	0x627, 0x623, 0x625, 0x622, 0x62f, 0x630, 0x631, 0x632, 
	0x648, 0x624, 0x629, 0x649
};
#endif

static pj_uint16_t theSet1[29]={
	0x626,
	0x628, 
	0x62a,
	0x62b, 
	0x62c,
	0x62d,
	0x62e,
	0x633, 
	0x634, 
	0x635, 
	0x636, 
	0x637, 
	0x638, 
	0x639, 
	0x63a, 
	0x641, 
	0x642,
	0x643, 
	0x644, 
	0x645, 
	0x646, 
	0x647,
	0x64a, 
	0x67e,  // Persian
	0x686,
	0x698,
	0x6a9,
	0x6af,
	0x6cc,
};

static pj_uint16_t theSet2[41]={
	0x622,
	0x623,
	0x624,
	0x625,
	0x626,
	0x627,
	0x628,
	0x629,
	0x62a, 
	0x62b,
	0x62c,
	0x62d,
	0x62e,
	0x62f,
	0x630,
	0x631,
	0x632, 
	0x633,
	0x634,
	0x635,
	0x636,
	0x637,
	0x638,
	0x639,
	0x63a,
	0x641,
	0x642, 
	0x643,
	0x644,
	0x645,
	0x646,
	0x647,
	0x648,
	0x649,
	0x64a,
	0x67e,  // Persian
	0x686,
	0x698,
	0x6a9,
	0x6af,
	0x6cc,
};



static pj_uint16_t theSet3[4]={
	0x622, 0x623, 0x625, 0x627
};

static pj_bool_t is_current_char_in_sets1(wchar_t cur_char)
{
	int i;

	for (i = 0; i < PJ_ARRAY_SIZE(theSet1); i ++)
	{
		if (cur_char == theSet1[i])
		{
			return PJ_TRUE;
		}
	}

	return PJ_FALSE;
}

static pj_bool_t is_current_char_in_sets2(wchar_t cur_char)
{
	int i;

	for (i = 0; i < PJ_ARRAY_SIZE(theSet2); i ++)
	{
		if (cur_char == theSet2[i])
		{
			return PJ_TRUE;
		}
	}

	return PJ_FALSE;
}

static int is_current_char_in_sets3(wchar_t cur_char)
{
	int i;

	for (i = 0; i < PJ_ARRAY_SIZE(theSet3); i ++)
	{
		if (cur_char == theSet3[i])
		{
			return i;
		}
	}

	return -1;
}

static pj_bool_t is_in_persian_set(wchar_t ptr)
{
	switch(ptr) {
		case 0x067e:
		case 0x0686:
		case 0x0698:
		case 0x06a9:
		case 0x06af:
		case 0x06cc:
			return PJ_TRUE;
	}
	return PJ_FALSE;
}

static void set_persian_tmp(wchar_t *tmp, int pos)
{
	switch(*tmp) {
		case 0x067e:
			*tmp = Persian_Position[0][pos];
			break;
		case 0x0686:
			*tmp = Persian_Position[1][pos];
			break;
		case 0x0698:
			*tmp = Persian_Position[2][pos];
			break;
		case 0x06a9:
			*tmp = Persian_Position[3][pos];
			break;
		case 0x06af:
			*tmp = Persian_Position[4][pos];
			break;
		case 0x06cc:
			*tmp = Persian_Position[5][pos];
			break;
	}
}

static int process_arabic_write_rule(wchar_t * string, int *len)
{
#define MAX_SUBSEQ 128
	wchar_t *tmp, start=L'*';
	wchar_t pleft, *pright, last_char;

	pleft = start;
	tmp = string;
	pright = tmp+1;

	last_char = *tmp;

	do 
	{
		last_char = *tmp;
		if ((*tmp >= 0x0621 && *tmp <= 0x064a) || is_in_persian_set(*tmp))
		{

			if (*tmp == 0x0644)
			{
				int index = is_current_char_in_sets3(*pright);
				if (index != -1)
				{
					last_char = *(tmp+1);
					if (is_current_char_in_sets1(pleft) == PJ_TRUE)
					{
						*tmp = arabic_specs[index][1];
					}
					else
					{
						*tmp = arabic_specs[index][0];
					}
					pj_array_erase(string, sizeof(wchar_t), *len, pright-string);
					*(string + (*len) - 1) = L'\0' ;
					(*len)--;
					goto STEP_NEXT;
				}
			}

			if (is_current_char_in_sets1(pleft) == PJ_TRUE)
			{
				if (is_current_char_in_sets2(*pright) == PJ_TRUE)
				{
					if (is_in_persian_set(*tmp)) {
						set_persian_tmp(tmp, 2);
					}else {
						*tmp = Arbic_Position[*tmp-0x0621][2];//middle
					}
				} 
				else
				{
					if (is_in_persian_set(*tmp)) {
						set_persian_tmp(tmp, 0);
					}else {
						*tmp = Arbic_Position[*tmp-0x0621][0];//last
					}
				}
			} 
			else
			{
				if (is_current_char_in_sets2(*pright) == PJ_TRUE)
				{
					if (is_in_persian_set(*tmp)) {
						set_persian_tmp(tmp, 1);
					}else {
						*tmp = Arbic_Position[*tmp-0x0621][1];//first
					}
				} 
				else
				{
					if (is_in_persian_set(*tmp)) {
						set_persian_tmp(tmp, 3);
					}else {
						*tmp = Arbic_Position[*tmp-0x0621][3];//alone
					}
				}
			}
		}

STEP_NEXT:

		pleft = last_char;
		tmp ++;
		pright = tmp +1;
	} while (*tmp != L'\0');

	return 0;
}

wchar_t text[2048] = {0};
unsigned int max_unicode = 0;
unsigned int min_unicode = 0xffffffff;

static void collect_char(wchar_t ch)
{
	int wlen;
	int i;
	unsigned int tmp;

	wlen = wcslen(text);
	if(wlen >= 1023) {
		printf("===============================yi chu\n");
		return;
	}
	for(i=0; i<wlen; i++) {
		if(text[i] == ch) {
			return;
		}
	}
	text[i] = ch;
	tmp = (unsigned int)ch;
	if(tmp > max_unicode) {
		max_unicode = tmp;
	}
	if(tmp < min_unicode){
		min_unicode = tmp;
	}
}

static void make_wchar_to_hex_string(char *dest, const wchar_t *src)
{
	int i, wlen;
	char tmp[16];

	wlen = wcslen(src);
	for(i=0; i<wlen; i++) {
		memset(tmp, 0, 16);
		sprintf(tmp, "0x%x, ", (unsigned int)src[i]);
		strcat(dest, tmp);

		//�ռ������õ����ַ�
		if(src[i] > 0x7e) {
			collect_char(src[i]);
		}
	}

}

static int _language_get_strings(int language_id, MXML_NODE *root_node, const char *str_name)
{
	MXML_NODE *tmpnode = NULL;
	wchar_t str_value[1024] = {0};
	char hex_value[1024] = {0};
	char str_content[1024] = {0};
	static int id_count = 0x1000;

	if(tmpnode = xml_findchild(root_node, str_name))
	{
		if (tmpnode->data != NULL)
		{
			int len = strlen(tmpnode->data);
			int wlen = pj_ansi_to_unicode_len(tmpnode->data, len);

			if (wlen == -1) {
				return -1;
			}

			pj_ansi_to_unicode(tmpnode->data, len, str_value, wlen*sizeof(wchar_t));
			str_value[wlen] = L'\0';

#if 0
			if (language_id == FARSI)
			{
				process_arabic_write_rule(str_value, &wlen);
			}

			if (is_cur_language_show_from_right_to_left(language_id))
			{
				int i;

				for (i = 0; i < wlen; i ++)
				{
					if (str_value[i] > 128)
					{//need reversal
						wchar_t temp, *start = str_value, *end = str_value + wlen - 1;

						while(end > start)
						{
							temp = *start;
							*start = *end;
							*end = temp;
							--end;
							++start;
						}
						break;
					}
				}
			}
#endif
		}

		//write arr file
		make_wchar_to_hex_string(hex_value, str_value);
		//sprintf(str_content, "wchar_t *str_%s[LANGUAGE_ARRAY_SIZE] = {strarray_%s,};\n", array_name, array_name);
		sprintf(str_content, "static const wchar_t str_%s[] = {%s0x0};\n", str_name, hex_value);
		fwrite(str_content, 1, strlen(str_content), arr_file);

		//write define file
		memset(str_content, 0, 1024);
		sprintf(str_content, "#define ID_%s    0x%x\n", str_name, id_count);
		fwrite(str_content, 1, strlen(str_content), define_file);
		id_count++;

		//write id file
		memset(str_content, 0, 1024);
		sprintf(str_content, "{ID_%s, str_%s},\n", str_name, str_name);
		fwrite(str_content, 1, strlen(str_content), id_file);

		return 0;
	}

	return -1;
}


int phone_load_language_strings(const char *config_file)
{
	MXML_DOCUMENT *doc;
	MXML_NODE *root_node;
	MXML_ATTRIBUTE *attr;
	int language_id;
	char language_name[128] = {0};
	char language_version[128] = {0};
	
	doc = mxml_read(config_file, 0);
	if(!(doc && doc->status == MXML_STATUS_OK))
		goto ERR;

	if((root_node = xml_findchild(doc->root,"Language")) == NULL)
		goto ERR;

	if((attr = xml_find_nodeattr(root_node, "version")) != NULL)
	{
		xml_get_attrdata_str(attr, language_version, sizeof(language_version));
	}

	if((attr = xml_find_nodeattr(root_node, "Type")) != NULL)
		language_id = xml_get_attrdata_int(attr);
	else
		goto ERR;

	if((attr = xml_find_nodeattr(root_node, "Name")) != NULL)
		xml_get_attrdata_str(attr, language_name, sizeof(language_name));
	else
		goto ERR;


	if (_language_get_strings(language_id, root_node, "PreDial") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Dialing") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Calling") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Connected") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Ringing") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Holding") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Peer_hold") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Trying_to_hold") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Trying_to_resume") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Answering") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Conference") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Conference_With") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Transferring") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Transfer_To") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Blind_Transfer") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Call_Ended") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Enter_Number") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "To") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "From") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Enter_Bxfer_Target") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Enter_PickUp_Number") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Enter_Park_Code") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Enter_UnPark_Code") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Retrieving") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Select_Pickup_Number") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Retrieving_in_Group") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "No_Ringing_In_Group") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Enter_Message") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "NewCall") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "EndCall") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Send") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "EndConf") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "More") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Exit") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "select") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Add") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Save") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Cancel") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Back") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Dial") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Xfer") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Bxfer") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Conf") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "join") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "hold") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "lcr") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "miss") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "delete") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "edit") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "ok") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "delChr") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "clear") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "alpha") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "num") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "IP") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "option") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "copy") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "paste") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "modify") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "yn") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "redial") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Directory") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "history") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "dnd") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "dnd_enable") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "cfwd") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "forward") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "cfwd_enable") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "pick") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "GrPick") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "park") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "unpark") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "SMS") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "reply") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "play") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "new") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "view") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Switch") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Answer") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Reject") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Resume") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Change") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "InComingCall") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Ext_1_contacting_stun_error") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Ext_2_contacting_stun_error") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Ext_3_contacting_stun_error") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Ext_4_contacting_stun_error") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Initializing_network") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Checking_DNS") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "LAN_Disconnected") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "WAN_Disconnected") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "IP_Conflict") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "New_Missed_Call") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "New_Missed_Calls") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Do_Not_Disturb") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Calls_Forwarded") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "PPPOE_dialing") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Menu") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Enable") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Disable") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Empty") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Activation") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "List_is_Empty") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Preference") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Headset") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Speaker") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Yes") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "No") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "No_service") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Factory_Reset") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Choice_Symbol") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "New") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Old") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Default") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Language_setting") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "New_Entry") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Number") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Name") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Ring") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Phone_book_is_full_Please_remove_a_record_info") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Call_History") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Dialed_Calls") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Missed_Calls") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Answered_Calls") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "NAT") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "WAN") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "LAN") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "dhcp") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Static") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "PPPoe") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Host_Name") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Domain") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Network") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Connect_Type") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Current_IP") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Current_Netmask") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Current_Gateway") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Current_primary_dns") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Current_secondary_dns") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Static_IP_Address") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Static_Subnet_Mask") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Static_Default_Route") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Static_DNS_1") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Static_DNS_2") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Wan_ip_is_null") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Wan_Subnet_Mask_is_null") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Wan_default_route_is_null") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Wan_IP_addr_error") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Wan_Subnet_Mask_error") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Wan_Default_Route_error") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Wan_Dns1_error") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Wan_Dns2_error") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Setting_error_pls_check") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Multicast_Address") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Bridge_Mode") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "VLAN") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Voice_VLAN_ID") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Data_VLAN_ID") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Voice_VLAN_ID_error") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Data_VLAN_ID_error") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Ext") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Sip_server") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Sip_port") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Sip_port_error") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Sipaccount") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "AuthID") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Display_Name") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Proxy") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "UserID") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "PPPOE_Login_Name") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "PPPOE_Login_Password") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "PPPOE_Service_Name") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Time_Date") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Date") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Time") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Time_zone") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Time_Mode") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "NTP") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "NTP_Settings") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Enable_NTP_Server") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "NTP_Server") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Manual") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Manual_Settings") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Time_Format") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Time_24hr") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Time_12hr") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Date_Format") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "day_month") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "month_day") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Date_y_m_d") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Time_h_m_s") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Date_format_error") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Time_format_error") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Conceal_Caller_ID") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Block_Anonymous_Call") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Call_Waiting") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Dial_Assistance") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Preferred_Audio_Device") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Secure_Call") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Auto_Answer") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Send_Audio_To_Speaker") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Call_Forward") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "CFWD_All_Number") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "CFWD_Busy_Number") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "CFWD_No_Ans_Number") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "CFWD_No_Ans_Delay") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Enable_Web_Server") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Set_Password") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "New_Password") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Confirm_Password") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Password_Disaccord") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Lcd_Contrast") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Lcd_Backlight") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Status") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Phone") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Registered") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Not_registered") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Product_Info") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Product_Name") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Serial_Number") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Software_Version") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Hardware_Version") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "MAC_Address") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Protocol") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Last_Reg_at") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Next_Reg_at") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Tone") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Ring_Tone") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "User_Define_1") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "User_Define_2") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "No_Ring") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Ring_Picker") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Voice_Mail") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Voice_Mail_list") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Voice_Mail_Number") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Password") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Password_Error") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Volume") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Handset_Volume") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Speaker_Volume") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Headset_Volume") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Ringer_Volume") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "rebooting") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "restarting") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Reboot") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Confirm_System_Reboot") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Confirm_System_Restart") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Reset_succeed_rebooting") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "All_parameters_will_be_reset_to_factory_default_values_info") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Load_channel_failed") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Downloading_firmware") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Erasing") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Upgrading") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "About_2_minutes") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "About_1_5_minutes") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "About_1_minutes") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "About_30_seconds") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Erase_Finish_Upgrade") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Upgrade_succeed") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Downloading_file") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Download_fw_ok") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Failed_once") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Failed_twice") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Failed_last_time") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Upgrade_failed") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "File_size_not_correct") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Wait_update") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "VPN_connect_failed") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Contacts") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "None") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Line") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "BLF") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "SpeedDial") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "DTMF") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Mode") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "LineKey") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "ProgramKey") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Module") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Key") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Account") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "None_explain") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Line_explain") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "BLF_explain") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "SpeedDial_explain") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "DTMF_explain") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Lan_Activation") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "PC_Activation") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Lan_Identifier") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "PC_Identifier") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Lan_Priority") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "PC_Priority") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Lan_Identifier_error") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "PC_Identifier_error") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Lan_Priority_error") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "PC_Priority_error") < 0)
		goto ERR;

	// ������Ϣ��ʶ��
	if (_language_get_strings(language_id, root_node, "WIFI") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Ethernet") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Connect_Mode") < 0)
		goto ERR;
	
	if (_language_get_strings(language_id, root_node, "SSID") < 0)
		goto ERR;
	
	if (_language_get_strings(language_id, root_node, "WIFI_Status") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "WIFI_Connected") < 0)
		goto ERR;
	
	if (_language_get_strings(language_id, root_node, "Off_Line") < 0)
		goto ERR;
	
	if (_language_get_strings(language_id, root_node, "Failed") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "IP_setting") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "WIFI_setting") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "AP_setting") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Hotspot_list") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Manual_add") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Link_WIFI") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Link") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Unlink") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Unsave") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Info") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Encrypt") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Signal") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Channel") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Ap_switch") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "DHCP_setting") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "On") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Off") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Ap_ssid") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Ap_password") < 0)
		goto ERR;
	
	if (_language_get_strings(language_id, root_node, "WiFi_prompt_msg") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Authentication") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Connection_successful") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Connection_fail") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "WLAN") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Security") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Ap_status") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Connect") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Signal_strength") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Excellent") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Good") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Fair") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Poor") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "IP_address") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Saved") < 0)
		goto ERR;

	if (_language_get_strings(language_id, root_node, "Not_range") < 0)
		goto ERR;
    if (_language_get_strings(language_id, root_node, "Search") < 0)
        goto ERR;
    if (_language_get_strings(language_id, root_node, "LDAP") < 0)
        goto ERR;
    if (_language_get_strings(language_id, root_node, "Remote_phone_book") < 0)
        goto ERR;
    if (_language_get_strings(language_id, root_node, "local_directory") < 0)
        goto ERR;
	if (_language_get_strings(language_id, root_node, "IP_Call") < 0)
		goto ERR;
	if (_language_get_strings(language_id, root_node, "IP_Calling") < 0)
		goto ERR;
	if (_language_get_strings(language_id, root_node, "Keypad_Lock") < 0)
		goto ERR;
	if (_language_get_strings(language_id, root_node, "keypad_lock_enable") < 0)
		goto ERR;
	if (_language_get_strings(language_id, root_node, "keypad_lock_type") < 0)
		goto ERR;
	if (_language_get_strings(language_id, root_node, "keypad_lock_timeout") < 0)
		goto ERR;
	if (_language_get_strings(language_id, root_node, "Phone_Unlock_Pin") < 0)
		goto ERR;
	if (_language_get_strings(language_id, root_node, "Menu_key") < 0)
		goto ERR;
	if (_language_get_strings(language_id, root_node, "Function_key") < 0)
		goto ERR;
	if (_language_get_strings(language_id, root_node, "All_key") < 0)
		goto ERR;
	if (_language_get_strings(language_id, root_node, "Answer_call_only") < 0)
		goto ERR;
	if (_language_get_strings(language_id, root_node, "emergency_call_only") < 0)
		goto ERR;
	if (_language_get_strings(language_id, root_node, "Reset_to_Factory_Warning") < 0)
		goto ERR;
	if (_language_get_strings(language_id, root_node, "Reset_Option") < 0)
		goto ERR;
	if (_language_get_strings(language_id, root_node, "Full_Reset") < 0)
		goto ERR;
	if (_language_get_strings(language_id, root_node, "Config_Reset") < 0)
		goto ERR;
	if (_language_get_strings(language_id, root_node, "UserData_Reset") < 0)
		goto ERR;
	if (_language_get_strings(language_id, root_node, "reset_to_factory_by_the_mode_of_Full_Reset") < 0)
		goto ERR;
	if (_language_get_strings(language_id, root_node, "reset_to_factory_by_the_mode_of_Config_Reset") < 0)
		goto ERR;
	if (_language_get_strings(language_id, root_node, "reset_to_factory_by_the_mode_of_UserData_Reset") < 0)
		goto ERR;
	mxml_node_destroy(doc->root);
	mxml_document_destroy(doc);
	doc = NULL;

	return 0;

ERR:
	if(doc)
	{
		if (doc->root != NULL)
		{
			mxml_node_destroy(doc->root);
		}
		mxml_document_destroy(doc);
		doc = NULL;
	}

	return -1;
}


int main(void)
{
	char config_file1[PJ_MAXPATH] = {0};
	char config_file2[PJ_MAXPATH] = {0};
	char config_file3[PJ_MAXPATH] = {0};
	char config_file4[PJ_MAXPATH] = {0};
	char config_file5[PJ_MAXPATH] = {0};

	char collect_text[3000] = {0};

	get_current_dir(config_file1);
	strcpy(config_file2, config_file1);
	strcpy(config_file3, config_file1);
	strcpy(config_file4, config_file1);
	strcpy(config_file5, config_file1);


	strcat(config_file1, "language_strings.xml");
	strcat(config_file2, "language_arrat.xml");
	strcat(config_file3, "language_define.xml");
	strcat(config_file4, "language_id.xml");
	strcat(config_file5, "char_language.xml");

	arr_file = fopen(config_file2, "w");
	if(arr_file == NULL) {
		printf("open language_arrat.xml error\n");
		goto out1;
	}

	define_file = fopen(config_file3, "w");
	if(define_file == NULL) {
		printf("open language_define.xml error\n");
		goto out2;
	}

	id_file = fopen(config_file4, "w");
	if(id_file == NULL) {
		printf("open language_id.xml error\n");
		goto out3;
	}

	char_file = fopen(config_file5, "w");
	if(char_file == NULL) {
		printf("open char_language.xml error\n");
		goto out3;
	}

	phone_load_language_strings(config_file1);

	pj_unicode_to_ansi(text, wcslen(text), collect_text, sizeof(collect_text));
	fwrite(collect_text, 1, strlen(collect_text), char_file);
	printf("-----------max:%x\n", max_unicode);
	printf("-----------min:%x\n", min_unicode);

	fclose(char_file);
	fclose(id_file);
out3:
	fclose(define_file);
out2:
	fclose(arr_file);
out1:
	return 0;
}
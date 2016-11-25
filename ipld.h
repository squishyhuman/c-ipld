#ifndef IPLD_HEADER_H

#define IPLD_HEADER_H

#include <stdio.h>
#include <string.h>
#include <jansson.h>
#include "base58.h"
#include "varhexutils.h"
//Predefined values:
#define IDKey "@id"
#define TypeKey "@type"
#define ValueKey "@value"
#define CtxKey "@/home/xethyrion/Desktop/Bak/varint.hcontext"
#define CodecKey "@codec"
#define LinkKey "mlink"

int NODE_H_DEBUG = 0; //Turn this on if you want to debug the program

/* Node struct
 * obj = json_t A jansson json object.
 * node_size = size of the node
*/
struct NODE
{
	json_t * obj;
	size_t node_size;
};
/* LOAD_NODE(@param1)
 * Creates a new node from a string.
 * @Param1 a json string(char *)
 * returns a json_t /home/xethyrion/Desktop/Bak/varint.hobject! (jansson.h)
*/
struct NODE LOAD_NODE(char * str)
{
	struct NODE X;
	json_error_t error;
	X.obj = json_loads(str, 0, &error);
	X.node_size = strlen(str);
	return X;
}
/* Unload_Node(@pa	for(int i=0; i<linknum; i++)
	{
		printf("AT %d %s\n",i,links[i]);
	}ram1)
 * Well, really easy, after being done with a struct if you don't want memory leaks..
 * you're supposed to free them, so use this to unload the node you create.
 * @param1 = node you previously created with NEW_NODE
*/ 
void UNLOAD_NODE(struct NODE X)
{
	json_decref(X.obj);
}
/* ntype(@param1, @param2)
 * @param1 = the string which will hold the result.
 * @param2 = NODE.object to get result from.
 * sets param 1 to @type, if any, stored as char* (supposed to be mlink!)
 * also sets str to NULL for your error checking purposes.
*/
void nType(char * str, json_t * O)
{
	json_t * proc;
	proc = json_object_get(O, TypeKey);
	if(proc)
	{
		json_t * value;
		const char * key;
		json_object_foreach(O, key, value)
		{
			if(strcmp(key, TypeKey) == 0)
			{
				const char * sval;
				sval = json_string_value(value);
				//DEBUG
				if(NODE_H_DEBUG == 1)
				{
					printf("Value: %s\nLength: %lu\n", sval, strlen(sval));
				}
				//END_DEBUG
				for(int i=0; i<=strlen(sval); i++)
				{
					str[i] = sval[i];
					if(i==strlen(sval))
					{
						str[i] = '\0';
					}
				}
			}
		}
	}
	else
	{
		str = NULL; //For your own error checking purposes
	}
}
/* nContext(@oaram1, @param2)
 * Context is a convenience method to retrieve the JSON-LD-style context
 * @Param1 = char * to hold result
 * @Param2 = NODE.obj to get result from
 * returns NULL if nothing
*/
void nContext(char * str, json_t * O)
{
	json_t * proc;
	proc = json_object_get(O, CtxKey);
	if(proc)
	{
		json_t * value;
		const char * key;
		json_object_foreach(O, key, value)
		{
			if(strcmp(key, CtxKey) == 0)
			{
				const char * sval;
				sval = json_string_value(value);
				//DEBUG
				if(NODE_H_DEBUG == 1)
				{
					printf("Value: %s\nLength: %lu\n", sval, strlen(sval));
				}
				//END_DEBUG
				for(int i=0; i<=strlen(sval); i++)
				{
					str[i] = sval[i];
					if(i==strlen(sval))
					{
						str[i] = '\0';
					}
				}
			}
		}
	}
	else
	{
		str = NULL; //For your own error checking purposes
	}
}
/*nLinks(@param1, @param2)
 *@Param1 = pointer array to store results in. (doesn't handle memory allocation!)
 *@Param2 = object to get results from!
 *
 *RETURNS LINK AMMOUNTS VERY IMPORTANT FOR FREEING THE MEMORY.
 * VERY IMPORTANT! always free your entire pointer array!
 * should be used as such size_t a = nlinks(@param1,@param2)
*/
size_t nLinks(char ** links, json_t * O)
{
	size_t links_ammount;
	links_ammount = 0;
	json_t * value;
	const char * key;
	json_object_foreach(O, key, value)
	{
		char * checker = NULL;
		checker = json_dumps(value, JSON_INDENT(0));
		//DEBUG
		if(NODE_H_DEBUG == 1)
		{		
			printf("Checker: %s\n", checker);
		}
		//END_DEBUG
		if(!checker && strcmp(key, TypeKey) == 0 && strcmp(json_string_value(value), "mlink") == 0)
		{
			printf("Warning: Premature Link Found!\n");
		}
		else if(checker)
		{
			//This key is the name of the field.
			const char * keyname = key;
			//DEBUG
			if(NODE_H_DEBUG == 1)
			{	
				printf("CHECK ACTIVE LINK: %s\n", key);
			}
			//END_DEBUG
			//checking individual fields for correct format.
			const char * typeoflink;
			typeoflink == NULL;
			const char * valueoflink;
			valueoflink == NULL;
			//foreachprep
			json_t * indvalue;
			const char * indkey;
			//endofprep
			json_object_foreach(value, indkey, indvalue)
			{
				char * checker2 = NULL;
				checker2 = json_dumps(indvalue, JSON_INDENT(0));
				//DEBUG
				if(NODE_H_DEBUG == 1)
				{
					printf("CHECKER_2: %s\n", checker2);
				}
				//END_DEBUG
				//I don't know how to do this yet so I'm letting this here for when I will.
				//Feel free to butt in guys :)
				if(checker2)//If another node is here.
				{
					//TO DO
				}
				else
				{
					if(strcmp(indkey, TypeKey) == 0)
					{
						typeoflink = indkey;
					}
					else if(strcmp(indkey, ValueKey) == 0)
					{
						valueoflink = indkey;
					}
				}
				
			}
			//DEBUG
			if(NODE_H_DEBUG == 1)
			{
				printf("Results: %s: %s && %s\n", keyname,typeoflink, valueoflink);
			}
			//END_DEBUG
			if(keyname && typeoflink && valueoflink)
			{
				links[links_ammount] = (char*)malloc(1000);
				bzero(links[links_ammount],1000);
				char * muhe = json_dumps(value, JSON_INDENT(0));
				sprintf(links[links_ammount], "{\"%s\": %s}", key,muhe);
				links_ammount++;
				free(muhe);
			}
		}
		free(checker);
	}
	return links_ammount;	
}
/* Link struct - Same as node, except made this way for easier structuring, better human readability
 * and avoiding memory errors/leaks
 * obj = json_t A jansson json object.
 * node_size = size of the node
*/
struct LINK
{
	int valid;
	json_t * obj;
	size_t link_size;
	char name[100];
	char type[100];
	char hash[100];
	char b58hash[1000];
};
/* lType(@param1,@param2)
 * Gets the type of the link, of course, it should be an mlink.
 * @param1: LINK.type //Storing in type from LINK struct.
 * @param2: LINK // The link structure we are using.
*/
void lType(char * str, struct LINK O)
{
	json_t * value;
	const char * key;
	json_object_foreach(O.obj, key, value)
	{
		json_t * value2;
		const char * key2;
		json_object_foreach(value, key2, value2)
		{
			if(strcmp(key2,TypeKey)==0)
			{
				const char * sval;
				sval = json_string_value(value2);
				//DEBUG
				if(NODE_H_DEBUG == 1)
				{
					printf("lType: %s\n", sval);
				}
				strcpy(str,sval);
			}
		}
	}
}

/* lHash(@param1,@param2)
 * Gets the hash of the link, not b58 decoded
 * @param1: LINK.hash //Storing in hash from LINK struct.
 * @param2: LINK // The link structure we are using.
*/
void lHash(char * str, struct LINK O)
{
	json_t * value;
	const char * key;
	json_object_foreach(O.obj, key, value)
	{
		json_t * value2;
		const char * key2;
		json_object_foreach(value, key2, value2)
		{
			if(strcmp(key2,ValueKey)==0)
			{
				const char * sval;
				sval = json_string_value(value2);
				//DEBUG
				if(NODE_H_DEBUG == 1)
				{
					printf("lHash: %s\n", sval);
				}
				strcpy(str,sval);
			}
		}
	}
}

/* lname(@param1,@param2)
 * Gets the name of the link {>Field1<:{}}
 * @param1: LINK.name //Storing in type from LINK struct.
 * @param2: LINK // The link structure we are using.
*/
void lName(char * str, struct LINK O)
{
	json_t * value;
	const char * key;
	json_object_foreach(O.obj, key, value)
	{
		if(NODE_H_DEBUG == 1)
		{
			printf("lName: %s\n", key);
		}
		strcat(str, key);
	}
}
/*B58Hash(@param1, @parunsigned char** binary_dataam2)
 *Decodes the hash, and stores it into a char from your link struct
*/
void lb58Hash(char * str, struct LINK X) //Need to find out if prefixed or not!
{
	char * hash = X.hash;
	size_t hash_length = strlen(hash);
	size_t result_buffer_length = libp2p_crypto_encoding_base58_decode_max_size(hash);
	unsigned char result_buffer[result_buffer_length];
	unsigned char * ptr_2_result = result_buffer;
	memset(result_buffer, 0, result_buffer_length);
	int valid = libp2p_crypto_encoding_base58_decode(hash,hash_length,&ptr_2_result, &result_buffer_length);
	printf("IS_VALID: %d\n",valid);
	char HReadable[1000];
	bzero(HReadable,1000);
	int ilen = 0;
	for(int i = 0; i<result_buffer_length;i++)
	{
		unsigned char c = ptr_2_result[i];
		char miu[3];
		bzero(miu,3);
		sprintf(miu,"%02x",c);
		miu[3] = '\0';
		strcat(HReadable, miu);
	}
	//DEBUG
	if(NODE_H_DEBUG == 1)
	{
		printf("Normal hash: %s\n", hash);
		printf("Result: %s\n", HReadable);
	}
	strcat(str, HReadable);
	
}
/* LOAD_LINK(@param1)
 * Creates a new LINK from a string.
 * @Param1 a json string(char *)
 * returns a json_t object! (jansson.h)
 * This is made to also give you easy access to any part of the link.
 * It will have LINK.valid = 0 if it's not a valid link, you should check for this always.
*/
struct LINK LOAD_LINK(char * str)
{
	struct LINK X;
	json_error_t error;
	X.obj = json_loads(str, 0, &error);
	X.link_size = strlen(str);
	bzero(X.type,100);
	bzero(X.name,100);
	bzero(X.hash,100);
	bzero(X.b58hash,1000);
	lName(X.name, X);
	lType(X.type, X);
	lHash(X.hash, X);
	lb58Hash(X.b58hash, X);
	return X;
}
/* Unload_LINK(@param1) - Same as unload_node, makes it easier to avoid leaks and better structuring of your programs
 * Well, really easy, after being done with a struct if you don't want memory leaks..
 * you're supposed to free them, so use this to unload the node you create.
 * this design makes it easier to read and mentain a good program architecture.
 * @param1 = node you previously created with NEW_NODE
*/
void UNLOAD_LINK(struct LINK X)
{
	json_decref(X.obj);
}
#endif
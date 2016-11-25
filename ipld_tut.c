#include "ipld.h"
int main()
{
	//TURN DEBUG MODE ON/OFF
	NODE_H_DEBUG = 0;
	
	//Load a node from string:
	char * str = "{\"@context\": \"/ipfs/Qmf1ec6n9f8kW8JTLjqaZceJVpDpZD4L3aPoJFvssBE7Eb/merkleweb\", \"@type\": \"node\", \"FIELD1\": { \"@value\": \"Qmf1ec6n9f8kW8JTLjqaZceJVpDpZD4L3aPoJFvssBE7Eb\", \"@type\": \"mlink\" }, \"FIELD2\": { \"@value\": \"Qmf1ec6n9f8kW8JTLjqaZceJVpDpZD4L3aPoJFvssBE7Eb\", \"@type\": \"mlink\" } }";
	struct NODE A;
	A = LOAD_NODE(str);
	
	//Get node contents back to a string:
	char * lol;
	lol = json_dumps(A.obj, JSON_INDENT(1) | JSON_PRESERVE_ORDER);
	printf("NODE CONTENTS: \n------------\n%s\n------------\n", lol);
	free(lol); // Never forget to free the char * after being done with it.

	//Get type of node, if any.
	char typeget[1000];   //Way bigger than needed.
	bzero(typeget,1000);  //No mem errors ofc.
	nType(typeget, A.obj);//Check for type if any
	printf("type: %s\n", typeget);
	
	//Get context of node.
	char ctxget[1000];    //Way bigger than needed
	bzero(ctxget, 1000);
	nContext(ctxget, A.obj);
	printf("ctx: %s\n", ctxget);

	//Extracting Links
	char * links[666];
	for(int i=0;i<666;i++){ links[i] = NULL;}
	size_t linknum;
	linknum = nLinks(links, A.obj); //These all need to be freed, hence the linknum!
	printf("Linknum: %lu\n", linknum);
	for(int i=0; i<linknum; i++)
	{
		printf("AT %d %s\n",i,links[i]);
	}
	
	//Store links in an more accessible version:
	struct LINK L;
	L = LOAD_LINK(links[0]);
	
	//By now you already have easy access to everything, the key name, the hash, the b58 decoded hash, the type.
	//It will reject any links which aren't of type "mlink", you should check this by seeing if it returns valid or not.
	char * linkstr;
	linkstr = json_dumps(L.obj, JSON_INDENT(1) | JSON_PRESERVE_ORDER);
	printf("LINK CONTENTS: \n------------\n%s\n------------\n", linkstr);
	free(linkstr);
	//Access Name
	printf("LINK NAME:\t%s\n", L.name);
	
	//Access Type
	printf("LINK TYPE:\t%s\n", L.type);
	
	//Access Hash
	printf("LINK HASH:\t%s\n", L.hash);
	
	//Access b58Hash
	printf("LINK B58HASH:\t%s\n", L.b58hash);
	
	//Free all nodes, links, link pointer array
	for(int i=0; i<linknum;i++){free(links[i]);} //FREE(Link char pointer array)
	UNLOAD_LINK(L); // FREE LINK STRUCTURE
	UNLOAD_NODE(A); // FREE NODE STRUCTURE // You could've done this earlier as soon as you didn't need any more.
	
	return 0;
}
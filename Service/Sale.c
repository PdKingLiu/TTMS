#include "../Persistence/Sale_Persist.h"
#include "../Persistence/Query_Persist.h"


//����Ʊ����Ϣ���뵽��������Ϣ
int Sale_Srv_Add(const sale_t *data) {
	return Sale_Perst_Insert(data);
}

//ɾ����������Ϣ
int Sale_Srv_DeleteByID(int saleID) {
	return Sale_Perst_DeleteByID(saleID);
}


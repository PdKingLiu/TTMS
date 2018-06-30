#include "../Persistence/Sale_Persist.h"
#include "../Persistence/Query_Persist.h"


//根据票的信息插入到订单的信息
int Sale_Srv_Add(const sale_t *data) {
	return Sale_Perst_Insert(data);
}

//删除订单的信息
int Sale_Srv_DeleteByID(int saleID) {
	return Sale_Perst_DeleteByID(saleID);
}


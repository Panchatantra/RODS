#define CONCAT(X,Y) X##Y
#define TO_STR(X) #X

#define JSON_TO_ELEMENTS(ELE_TYPE, PARA)\
auto j_array_##ELE_TYPE = model.at(TO_STR(ELE_TYPE##Vec));\
ELE_TYPE ELE_TYPE##Obj;\
for (auto i = 0; i<model.at(TO_STR(ELE_TYPE##Count)); i++)\
{\
    j_array_##ELE_TYPE[i].get_to(ELE_TYPE##Obj);\
    add##ELE_TYPE(ELE_TYPE##Obj.id, ELE_TYPE##Obj.IdDofI, ELE_TYPE##Obj.IdDofJ, ELE_TYPE##Obj.PARA);\
}

#define JSON_TO_ELEMENTS_2(ELE_TYPE, PARA1, PARA2)\
auto j_array_##ELE_TYPE = model.at(TO_STR(ELE_TYPE##Vec));\
ELE_TYPE ELE_TYPE##Obj;\
for (auto i = 0; i<model.at(TO_STR(ELE_TYPE##Count)); i++)\
{\
    j_array_##ELE_TYPE[i].get_to(ELE_TYPE##Obj);\
    add##ELE_TYPE(ELE_TYPE##Obj.id, ELE_TYPE##Obj.IdDofI, ELE_TYPE##Obj.IdDofJ,\
        ELE_TYPE##Obj.PARA1, ELE_TYPE##Obj.PARA2);\
}

#define JSON_TO_ELEMENTS_3(ELE_TYPE, PARA1, PARA2, PARA3)\
auto j_array_##ELE_TYPE = model.at(TO_STR(ELE_TYPE##Vec));\
ELE_TYPE ELE_TYPE##Obj;\
for (auto i = 0; i<model.at(TO_STR(ELE_TYPE##Count)); i++)\
{\
    j_array_##ELE_TYPE[i].get_to(ELE_TYPE##Obj);\
    add##ELE_TYPE(ELE_TYPE##Obj.id, ELE_TYPE##Obj.IdDofI, ELE_TYPE##Obj.IdDofJ,\
        ELE_TYPE##Obj.PARA1, ELE_TYPE##Obj.PARA2, ELE_TYPE##Obj.PARA3);\
}

#define ELEMENTS_TO_JSON(ELE_TYPE)\
model[TO_STR(ELE_TYPE##Count)] = ELE_TYPE##s.size();\
vector<ELE_TYPE> ELE_TYPE##Vec;\
for (auto it = ELE_TYPE##s.begin(); it != ELE_TYPE##s.end(); it++)\
    ELE_TYPE##Vec.push_back(*(it->second));\
model[TO_STR(ELE_TYPE##Vec)] = ELE_TYPE##Vec;
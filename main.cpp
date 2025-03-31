#include <iostream>
#include <cstring>

#define T 8
#define SPACJA 32

#define IATRYBUTQ 11
#define NATRYBUTQ 12
#define IATRYBUTN 13

#define ISELEKTORQ 14
#define ZSELEKTORQ 19
#define ISELEKTORJ 15

#define ZEN 16

#define ID 17
#define IDN 18
#define COMANDSEND 20

using namespace std;

class String {
public:
	int size;
	int capacity;
	char* napis;
	String() : size(0), capacity(2), napis(NULL) {
		this->napis = new char[capacity];
	}

	~String() {
		delete[]napis;
	}
	void append(char czar) {
		if (size < capacity) {
			napis[size] = czar;
			size++;
		}
		else {
			capacity *= 2;
			char* nowy_napis = new char[capacity];
			for (int i = 0; i <= size; i++)
			{
				nowy_napis[i] = napis[i];
			}
			nowy_napis[size] = czar;
			napis = nowy_napis;
			size++;
		}
	}
	void usuwanieString() {
		for (int i = size - 1; i >=0; i--)
		{
			napis[i] = NULL;
		}
		size = 0;
		capacity = 2;
	}
	void ladnyString() {
		int i = 0, j = size - 1;
		if(strcmp(this->napis,"")!=0){
		while (napis[i] == SPACJA) {
			i++;
		}
		while (napis[j] == SPACJA && j>=0) {
			j--;
		}
			int new_size = j - i + 1;
			if(new_size > 0){
				char* tmp = new char[new_size + 1];

				for (int k = 0; k < new_size; k++)
				{
					tmp[k] = napis[i];
					i++;
				}
				tmp[new_size] = '\0';
				for (int k = 0; k < new_size; k++)
				{
					napis[k] = tmp[k];
				}
				size = new_size;
				append('\0');
				delete[]tmp;
			}
		}
	}

	char& operator[](int index) {
		return napis[index];
	}

	const char& operator[](int index) const {
		return napis[index];
	}

	String& operator=(const String& right) {//swap

		if (this != &right) {
			char* tmp = new char[right.size + 1];
			for (size_t i = 0; i <= right.size; i++) {
				tmp[i] = right.napis[i];
			}
			delete[] napis;
			napis = tmp;
			size = right.size;
		}
		return *this;
	}

	String& operator=(String&& right) noexcept {//move
		if (this != &right) {
			delete[] napis;
			size = right.size;
			napis = right.napis;
			right.size = 0;
			right.napis = nullptr;
		}
		return *this;
	}
};
class Selector {
public:
	String name;
	Selector* prev;
	Selector* next;

	Selector():name(), prev(nullptr), next(nullptr) {}
	~Selector() {
		this->prev = nullptr;
		this->next = nullptr;
	}
};

class Attribute {
public:
	String name;
	String value;
	Attribute* prev;
	Attribute* next;
	Attribute() : name(), value(), prev(nullptr), next(nullptr) {}
	Attribute(String name,String value)
	{
		this->name = name;
		this->value = value;
		this->prev = nullptr;
		this->next = nullptr;
	}
	~Attribute() {
		this->next = nullptr;
		this->prev = nullptr;
	}
};

class SelectorList {
public:
	SelectorList():head(nullptr),tail(nullptr) {};
	~SelectorList() {

	}
	void insert(const String &value) {
		Selector* selektor = head;
		while (selektor != nullptr) {
			if (strcmp(value.napis, selektor->name.napis) == 0) {
				selektor->name = value;
				break;
			}
			selektor = selektor->next;

		}
		if (selektor == nullptr) {
			selektor = new Selector;
			selektor->name = value;
			selektor->next = nullptr;

			if (head == nullptr) {
				head = selektor;
				tail = selektor;
			}
			else {
				tail->next = selektor;
				selektor->prev = tail;
				tail = selektor;
			}
		}
	}
	
	Selector* head;
	Selector* tail;
};

class AttributeList {
public:
	AttributeList() :head(nullptr), tail(nullptr) {};
	~AttributeList() {
	}
	
	void insert(const String& name,const String& value) {
		Attribute* attribute = head;
		while (attribute != nullptr) {
			if (strcmp(name.napis, attribute->name.napis) == 0) { 
				attribute->value = value;
				break;
			}
			attribute = attribute->next;

		}
		if(attribute==nullptr){
			attribute = new Attribute;
			attribute->name = name;
			attribute->value = value;
			attribute->next = nullptr;

			if (head == nullptr) {
				head = attribute;
				tail = attribute;
			}
			else {
				tail->next = attribute;
				attribute->prev = tail;
				tail = attribute;
			}
		}
	}

	void usuwanieAtrybutu(Attribute* atrybut) {
		
		if ((atrybut == head) && (atrybut == tail)) {
			head = nullptr;
			tail = nullptr;
			delete atrybut;
		}
		else if (atrybut == head)
		{
			head = atrybut->next;
			atrybut->next->prev = nullptr;
			delete atrybut;
		}
		else if (atrybut == tail) {
			atrybut->prev->next = nullptr;
			tail = atrybut->prev;
			delete atrybut;
		}
		else {
			atrybut->next->prev = atrybut->prev;
			atrybut->prev->next = atrybut->next;
			delete atrybut;
		}
	}
	Attribute* head;
	Attribute* tail;
};

class Blok {
public:
	AttributeList attribute_list;
	SelectorList selector_list;
	Blok(){}
	~Blok() {
		
	}
};


class Section {
public:
	Blok* tablica[T];
	Section* prev;
	Section* next;
	int blok_counter;
	~Section() {

	}
};

class SectionList {
public:
	SectionList() :head(nullptr), tail(nullptr),size(0) {};
	~SectionList() {
		Section* current = head;
		while (current != nullptr) {
			Section* temp = current;
			current = current->next;
			delete temp;
		}
		head = nullptr;
		tail = nullptr;
	}
	int licznikSekcji() {
		int suma = 0;
		Section* tmp = head;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < T; j++) {
				if (tmp->tablica[j] == NULL)continue;
				else suma++;
			}
			tmp = tmp->next;
		}
		return suma;
	}
	int n_a_q(const String& text) {
		String nnn;
		int i = 0, suma=0;

		while (text[i] != ',') {
			nnn.append(text[i]);
			i++;
		}
		nnn.ladnyString();
		Section* tmpSection = new Section;
		tmpSection = head;
		Attribute* tmpAtrybut = new Attribute;
		while (tmpSection!=nullptr) {

			for (int j = 0; j < T; j++)
			{
				if (tmpSection->tablica[j] != NULL && tmpSection->tablica[j]!= NULL) {
					
					tmpAtrybut = tmpSection->tablica[j]->attribute_list.head;
					while (tmpAtrybut != nullptr)
					{
						if (strcmp(tmpAtrybut->name.napis, nnn.napis) == 0)suma++;
						tmpAtrybut = tmpAtrybut->next;
					}
				}
				else continue;
			}

			tmpSection = tmpSection->next;
		}
		delete tmpSection;
		delete tmpAtrybut;
		return suma;
	}
	int z_s_q(const String& text) {
		String nnn;
		int i = 0, suma = 0;

		while (text[i] != ',') {
			nnn.append(text[i]);
			i++;
		}
		nnn.ladnyString();
		Section* tmpSection = new Section;
		tmpSection = head;
		Selector* tmpSelector = new Selector;
		while (tmpSection != nullptr) {

			for (int j = 0; j < T; j++)
			{
				if (tmpSection->tablica[j]!=NULL &&tmpSection->tablica[j] != NULL) {
					
					tmpSelector = tmpSection->tablica[j]->selector_list.head;
					while (tmpSelector != nullptr)
					{
						if (strcmp(tmpSelector->name.napis, nnn.napis) == 0)suma++;
						tmpSelector = tmpSelector->next;
					}
				}
				else continue;
			}

			tmpSection = tmpSection->next;
		}
		return suma;
	}
	int i_a_q(const String& text) {
		int index = 0, liczba = 0;
		String tmpliczba;

		while (text[index] != ',') {
			tmpliczba.append(text[index]);
			index++;
		}

		index++;
		liczba = atoi(tmpliczba.napis);
		index = 0;
		
		if (head != nullptr) {
			Section* Section_tmp = head;
			while (Section_tmp != nullptr) {
				if (Section_tmp->tablica[index] != NULL)liczba--;
				if (liczba == 0)break;
				index++;
				if (index == T ) { Section_tmp = Section_tmp->next; index = 0; }
			}
			if (liczba == 0) {
				int suma=0;
				Attribute* atrybut = Section_tmp->tablica[index]->attribute_list.head;
				while (atrybut != nullptr) {
					suma++;
					atrybut = atrybut->next;
				}
				return suma;
			}
			else return -1;
		}
		else return -1;
	}	
	int i_s_q(const String& text) {
		int index = 0, liczba = 0;
		String tmpliczba;

		while (text[index] != ',') {
			tmpliczba.append(text[index]);
			index++;
		}

		index++;
		liczba = atoi(tmpliczba.napis);
		index = 0;
		
		if (head != nullptr) {
			Section* Section_tmp = head;
			while (Section_tmp != nullptr) {
				if (Section_tmp->tablica[index] != NULL)liczba--;
				if (liczba == 0)break;
				index++;
				if (index == T) { Section_tmp = Section_tmp->next; index = 0; }
			}
			if (liczba == 0) {
				int suma = 0;
				Selector* selektor = Section_tmp->tablica[index]->selector_list.head;
				while (selektor != nullptr) {
					if(selektor->name.napis[0]!='0')suma++;
					selektor = selektor->next;
				}
				return suma;
			}
			else return -1;
		}
		else return -1;
	}
	char* i_s_j(const String& text) {
		int index = 0,i,j;
		String itmp, jtmp;
		while (text[index] != ',') {
			itmp.append(text[index]);
			index++;
		}
		index++;
		i = atoi(itmp.napis);
		while (text[index] != ',') {
			index++;
		}
		index++;
		while (text[index])
		{
			jtmp.append(text[index]);
			index++;
		}
		j = atoi(jtmp.napis);
		Section* tmpSection = new Section;
		tmpSection = head;
		index = 0;

		if (head != nullptr) {
			Section* Section_tmp = head;
			while (Section_tmp != nullptr) {
				if (Section_tmp->tablica[index] != NULL)i--;
				if (i == 0)break;
				index++;
				if (index == T) { Section_tmp = Section_tmp->next; index = 0; }
			}
			if (i == 0) {
				Selector* selektor = Section_tmp->tablica[index]->selector_list.head;
				while (selektor != nullptr) {	
					
					j--;
					if (j == 0)break;
					selektor = selektor->next;
				}
				if (j == 0 && selektor->name.napis[0]!='0') {
					return selektor->name.napis;
				}
				else return NULL;
			}
			else return NULL;
		}
		else return NULL;
	}
	char* i_a_n(const String &text) {
		int i = 0;
		int liczba = 0;
		String tmpliczba,tmpn;
		
		while (text[i] != ',') {
			tmpliczba.append(text[i]);
			i++;
		}
		i++;
		liczba = atoi(tmpliczba.napis);
		while (text[i] != ',') {
			i++;
		}
		i++;
		while (text[i])
		{
			tmpn.append(text[i]);
			i++;
		}
		tmpn.ladnyString();
		i = 1;
		int k = 0;
		Section* tmpSection = new Section;
		tmpSection = head;
		while (i!=liczba) {
			if (k == T) {
				tmpSection = tmpSection->next;
				k = 0;
			}
			if (tmpSection == nullptr)return NULL;
			if (tmpSection->tablica[k]!=NULL && tmpSection->tablica[k] != NULL)i++;
			k++;
		}
		if (k == T) {
			tmpSection = tmpSection->next;
			k = 0;
		}
		Attribute* tmpAtrybut = new Attribute;
		if (tmpSection->tablica[k] == NULL)return NULL;
		tmpAtrybut = tmpSection->tablica[k]->attribute_list.head;
		
		while (tmpAtrybut!=nullptr)
		{
			if (strcmp(tmpAtrybut->name.napis, tmpn.napis)==0)return tmpAtrybut->value.napis;
			tmpAtrybut = tmpAtrybut->next;
		}
		if (tmpAtrybut == nullptr)return NULL;
		if (strcmp(tmpAtrybut->name.napis, tmpn.napis) == 0)return tmpAtrybut->value.napis;
		else return NULL;
	}
	
	char* z_e_n(const String& text) {
		String z,n;
		int index = 0, suma = 0;
		while (text[index] != ',') {
			z.append(text[index]);
			index++;
		}
		index++;
		while (text[index] != ',') {
			index++;
		}
		index++;
		while (text[index])
		{
			n.append(text[index]);
			index++;
		}
		n.ladnyString();//atrybut
		z.ladnyString();//selektor
		
		Section* Section_tmp = new Section;
		if (tail != nullptr) {
			//int index = T - 1;
			Section_tmp = tail;
			Attribute* Atrybut_tmp;
			Selector* Selektor_tmp;
			Section* Sekcja_tmp;
			Sekcja_tmp = tail;
			while (Section_tmp != nullptr) {
				for (int k = T - 1; k >= 0; k--) {
					if (Section_tmp->tablica[k] != NULL) {
						Selektor_tmp = Section_tmp->tablica[k]->selector_list.head;
						while (Selektor_tmp != nullptr) {
							if (strcmp(Selektor_tmp->name.napis, z.napis) == 0) {
								Atrybut_tmp = Section_tmp->tablica[k]->attribute_list.head;
								while (Atrybut_tmp != nullptr) {
									if (strcmp(Atrybut_tmp->name.napis, n.napis) == 0)return Atrybut_tmp->value.napis;
									Atrybut_tmp = Atrybut_tmp->next;
								}
							}
							Selektor_tmp = Selektor_tmp->next;
						}
					}
				}
				Section_tmp = Section_tmp->prev;
			}
			return NULL;

		}
		else return NULL;
	}
	
	int i_d_n(const String& text) {
		int index = 0, liczba;
		String tmpliczba,atrybut_nazwa;
		while (text[index] != ',') {
			tmpliczba.append(text[index]);
			index++;
		}
		index++;
		liczba = atoi(tmpliczba.napis);
		while (text[index] != ',') {
			index++;
		}
		index++;
		while (text[index])
		{
			atrybut_nazwa.append(text[index]);
			index++;
		}
		atrybut_nazwa.ladnyString();
		index = 0;
		if (head != nullptr) {
			Section* Section_tmp = head;
			while (Section_tmp != nullptr) {
				if (Section_tmp->tablica[index] != NULL)liczba--;
				if (liczba == 0)break;
				index++;
				if (index == T) { Section_tmp = Section_tmp->next; index = 0; }
			}
			if (liczba == 0) {
				Attribute* atrybut = Section_tmp->tablica[index]->attribute_list.head;
				while (atrybut != nullptr) {
					if (strcmp(atrybut->name.napis, atrybut_nazwa.napis) == 0) {

						Section_tmp->tablica[index]->attribute_list.usuwanieAtrybutu(atrybut);
						if (Section_tmp->tablica[index]->attribute_list.head == nullptr) {
							delete Section_tmp->tablica[index];
							Section_tmp->tablica[index] = NULL;
						}
						return 1;
					}
					atrybut = atrybut->next;
				}
				return 0;
			
			}
			else return 0;
		}
		else return 0;
	}

	int i_d(const String& text) {
		int index = 0, liczba;
		String tmpliczba;
		while (text[index] != ',') {
			tmpliczba.append(text[index]);
			index++;
		}
		liczba = atoi(tmpliczba.napis);
		index = 0;
		if (head != nullptr) {
			Section* Section_tmp = head;
			while (Section_tmp!=nullptr) {
				if (Section_tmp->tablica[index] != NULL)liczba--;
				if (liczba == 0)break;
				index++;
				if (index == T) { Section_tmp = Section_tmp->next; index = 0; }
			}
			if (liczba == 0) {
		
				for (int i = 0; i < T; i++) {
					if (Section_tmp->tablica[i] != NULL)liczba++;
				}
				if (liczba == 0) {
					usuwanieSekcji(Section_tmp);
				}
				else { 

					delete Section_tmp->tablica[index]; 
					Section_tmp->tablica[index] = NULL;
				}
				return 1;
			}
			else return 0;
		}
		else return 0;
		
	}
	
	void usuwanieSekcji(Section* sekcja) {
		if (sekcja == head && sekcja == tail) {
			this->~SectionList();
		}
		else if (sekcja == head)
		{
			head = sekcja->next;
			sekcja->next->prev = nullptr;
		}
		else if (sekcja == tail) {
			sekcja->prev->next = nullptr;
			tail = sekcja->prev;
		}
		else {
			sekcja->next->prev = sekcja->prev;
			sekcja->prev->next = sekcja->next;
		}
		delete sekcja;
		size--;
	}
	void insert(Blok* value) {
		value != NULL;
		if (size == 0){
			Section* sekcja = new Section();
			head = sekcja;
			tail = sekcja;
			sekcja->tablica[0] = value;
			sekcja->blok_counter++;
			size++;

		}
		else {
			if (tail->blok_counter == T) {
				Section* sekcja = new Section();
				tail->next = sekcja;
				sekcja->prev = tail;
				tail = sekcja;
				sekcja->tablica[0] = value;
				sekcja->blok_counter++;
				size++;
			}
			else
			{  
				tail->tablica[tail->blok_counter] = value;
				tail->blok_counter++;
			}
		}
	}

private:
	Section* head;
	Section* tail;

	size_t size;
};

int findComand(const String &text) {
	int i = 0;
	if (isspace(text[i])!=0)return NULL;
	while (text[i] != ',') {
		i++;
		if (text[i] == '*')return COMANDSEND;
	}
	i++;
	
	if (text[i] == 'A') {
		if (text[i + 2] == '?') {
			for (int j = 0; j < (i - 1); j++) {
				if (!isdigit(text[j])) return NATRYBUTQ;
			}
			return IATRYBUTQ;
		}
		else return IATRYBUTN;
	}
	
	else if (text[i] == 'S') {
		if (text[i + 2] == '?') {
			for (int j = 0; j < (i - 1); j++) {
				if (!isdigit(text[j])) return ZSELEKTORQ;
			}
			return ISELEKTORQ;
		}
		else return ISELEKTORJ;
	}
	
	else if (text[i] == 'E')
		return ZEN;
	
	else {
		if (text[i + 2] == '*')return ID;
		else return IDN;
	}
}

int main() {
	SectionList sekcja_lista;
	Blok *blok = new Blok;
	String selektor_string;
	String atrybut_string;
	String wartosc_string;
	String sekcja_string;
	String komenda;
	
	bool selector_end = false, nameofthevalue_end = false, comands = false,komenda_begin=false;


	char znak;
	//for(;;){
	while ((znak = getchar()) != EOF) {
		//znak = getchar();
		if (znak == '\t')continue;
		if(comands==false){
			if (znak == '\n')continue;
			
			//SPRAWDZANIE ZNAKÓW CHARAKTERYSTYCZNYCH
			if (znak == '}')
			{
				sekcja_lista.insert(blok);
				selector_end = false;
				blok = new Blok();
				continue;
			}
			if ((znak == '{' || (znak == ',') && selector_end == false)) {
				selektor_string.ladnyString();
				if (selektor_string[0] == '\0') {
					selektor_string.append('0'); selektor_string.append('\0');
				}
				blok->selector_list.insert(selektor_string);
				selektor_string.usuwanieString();
				if(znak=='{')selector_end = true;
				continue;
			}
			if (znak == ';') {
				wartosc_string.ladnyString();
				atrybut_string.ladnyString();
				blok->attribute_list.insert(atrybut_string, wartosc_string);
				atrybut_string.usuwanieString();
				wartosc_string.usuwanieString();
				nameofthevalue_end = false;
				continue;
			}
			if (znak == ':'&& selector_end==true) {
				nameofthevalue_end = true;
				continue;
			}	
			if (znak == '?') {
				for (int i = 0; i < 3; i++)
				{
					znak=(char)getchar();
					komenda.append(znak);
				}
				if (komenda[1] == '?' && komenda[2] == '?')
				{
					comands = true;
					komenda.usuwanieString();
					continue;
				}
			}

			//WCZYTYWANIE NAPISÓW
			if (selector_end == false) {
				selektor_string.append(znak);
			}
		
			if ((selector_end == true) && (nameofthevalue_end == false)) {
					atrybut_string.append(znak);
			}
			if ((selector_end == true) && (nameofthevalue_end == true)) {
				wartosc_string.append(znak);
			}
		}
		else
		{
			if (znak == '\n' && komenda_begin == false) {
				komenda_begin = true;
				continue;
			}
			if (znak == '\n' && komenda_begin == true) {
				komenda.ladnyString();
				if (komenda.size == 2 && komenda.napis[0]=='?') {
					cout << komenda.napis << " == " << sekcja_lista.licznikSekcji() << endl;
					komenda.usuwanieString();
				}
				else {
				if (komenda[0] == NULL) {
					 komenda.usuwanieString();
					 continue;
				}
					int com = findComand(komenda);
					
					if (com == COMANDSEND) {
						comands = false;
						komenda.usuwanieString();
						komenda_begin = false;
					}
					else if (com == IATRYBUTN) {
						char* x = sekcja_lista.i_a_n(komenda);
						if (x== NULL) {
							komenda.usuwanieString();
							continue;							
						}

						else cout << komenda.napis << " == " << x << endl;
					}
					else if (com == IATRYBUTQ) {
						int x = sekcja_lista.i_a_q(komenda);
						if (x == -1) {
							komenda.usuwanieString();
							continue;							
						}
						else cout << komenda.napis << " == " << x << endl;
					}
					else if (com == NATRYBUTQ) {
						int x = sekcja_lista.n_a_q(komenda);
						if (x == -1) {
							komenda.usuwanieString();
							continue;							
						}
						else cout << komenda.napis << " == " << x << endl;
					}
					else if (com == ISELEKTORQ) {
						int x = sekcja_lista.i_s_q(komenda);
						if (x == -1) {
							komenda.usuwanieString();
							continue;
						}
						else cout << komenda.napis << " == " << x << endl;
					}
					else if (com == ZSELEKTORQ) {
						int x = sekcja_lista.z_s_q(komenda);
						if (x == -1) {
							komenda.usuwanieString();
							continue;							
						}
						else cout << komenda.napis << " == " << x << endl;
					}
					else if (com == ISELEKTORJ) {
						char* x = sekcja_lista.i_s_j(komenda);
						if (x== NULL) {
							komenda.usuwanieString();
							continue;
						}
						else cout << komenda.napis << " == " << x << endl;
					}
					else if (com == ZEN) {
						char* x = sekcja_lista.z_e_n(komenda);
						if (x == NULL) {
							komenda.usuwanieString();
							continue;
						}
						else cout << komenda.napis << " == " << x << endl;
					}
					else if (com == ID) {
						int x = sekcja_lista.i_d(komenda);
						if (x == NULL) {
							komenda.usuwanieString();
							continue;
						}
						else cout << komenda.napis << " == deleted" << endl;
					}
					else if (com == IDN) {
						int x = sekcja_lista.i_d_n(komenda);
						if (x == NULL) {
							komenda.usuwanieString();
							continue;
						}
						else cout << komenda.napis << " == deleted" <<endl;
					}
					else {
						komenda.usuwanieString();
						continue;
					}
					komenda.usuwanieString();	
				}
			}
			if(znak!='\n'){
				komenda.append(znak);
			}
		}
	}
	komenda.ladnyString();
	if (komenda.size == 2 && komenda.napis[0] == '?') {
		cout << komenda.napis << " == " << sekcja_lista.licznikSekcji() << endl;
		komenda.usuwanieString();
	}
	delete blok;
	return 0;
}
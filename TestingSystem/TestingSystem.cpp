#include <iostream>
#include <string>
#include <stdexcept>
/*
		Задание 1: Система тестирования
		+ модуль регистрации
			+ пользователь вводит имя, логин и пароль (2 раза)
			+ если второй ввод пароля не совпал с первым - выводим ошибку
		+ модуль аутентификации

			+ пользователь вводит логин и пароль который он указал при
			  регистрации
			+ в случае успешного ввода логина и пароля выводим приветствие
			  пользователя по имени. Например: Hello, Mike.
		+ модуль тестов
			+ пользователь выбирает категорию тестов (всего 2 категории)
			+ далее пользователь проходит заранее заготовленный тест
			  (количество вопросов в тесте можете установить самостоятельно)
			+ после прохождения теста покажите пользователю
			  количество правильных и неправильных ответов

					ОБНОВЛЕНИЯ
			+ Новый тип пользователя: Админ.
			+ Если в систему зашел админ, то он должен иметь возможность
			+ регистрировать новых пользователей.

			+ При работе с программной должна быть возможность сменить аккаунт
			  на иного пользователя, например на того, которого зарегистрировал
			  админ
	*/

class Ekzam;

struct User_data
{
	enum ROLE { USER, ADMIN };

	std::string m_name;
	std::string m_login;
	std::string m_password;
	std::string m_repeat_password;
	ROLE m_role;

	void set_role(const ROLE& role)
	{
		m_role = role;
	}

	const ROLE& get_role()const
	{
		return m_role;
	}

	friend  std::istream& operator>>(std::istream& is, User_data& n)
	{
		std::cout << "\n\nEnter name     : ";
		std::getline(is, n.m_name);

		std::cout << "Enter login    : ";
		std::getline(is, n.m_login);

		std::cout << "Enter password : ";
		std::getline(is, n.m_password);

		std::cout << "Repeat password : ";
		std::getline(is, n.m_repeat_password);
		n.m_role = User_data::USER;
		return is;
	}
};

class Users
{
public:
	Users() = default;

	bool add(const User_data& user)
	{
		if (m_size == m_index)capacity_up();

		if (authentication(user.m_login, user.m_password)) {
			return false;
		}

		m_users[m_index++] = user;

		return true;
	}

	void pass_admin(const int index)
	{
		for (int i = 0; i < m_index; ++i)
		{
			if (i == index)
				m_users[i].set_role(User_data::ADMIN);
			else
				m_users[i].set_role(User_data::USER);
		}
	}

	void deleted(const int index)
	{
		if (m_index <= index)
		{
			User_data* tmp = new User_data[m_index - 1];
			for (int i = 0, b = 0; i < m_index; ++i)
			{
				if (i != index) {
					tmp[b] = m_users[i];
					b++;
				}
			}
			delete[] m_users;
			m_index--;
			m_users = tmp;
		}
		else {
			throw std::range_error("Out of range!");
		}
	}

	bool authentication(const std::string& login, const std::string& password)
	{
		for (int i = 0; i < m_index; ++i) {
			if (m_users[i].m_password == password && m_users[i].m_login == login)
				return true;
		}
		return false;
	}

	User_data::ROLE get_role(const int index)
	{
		return m_users[index].m_role;
	}

	const User_data& get_user(const std::string& login, const std::string& password)
	{
		for (int i = 0; i < m_index; ++i)
		{
			if (m_users[i].m_login == login && m_users[i].m_password == password)
			{
				return m_users[i];
			}
		}
		throw std::invalid_argument("Not user!");
	}

	~Users()
	{
		delete[] m_users;
	}

	Users(const Users& other)
	{
		m_index = other.m_index;
		m_size = other.m_size;
		m_users = new User_data[m_size];

		for (size_t i = 0; i < m_index; i++)
			m_users[i] = other.m_users[i];
	}

	Users(Users&& other)
	{
		m_index = other.m_index;
		m_size = other.m_size;
		m_users = other.m_users;
		other.m_users = nullptr;
	}

	Users& operator=(const Users& other)
	{
		m_index = other.m_index;
		m_size = other.m_size;
		m_users = new User_data[m_size];

		for (size_t i = 0; i < m_index; i++)
			m_users[i] = other.m_users[i];

		return *this;
	}

	Users& operator=(Users&& other)
	{
		m_index = other.m_index;
		m_size = other.m_size;
		m_users = other.m_users;
		other.m_users = nullptr;

		return *this;
	}

private:

	friend std::ostream& operator<<(std::ostream& os, const Users& users)
	{
		os << "#\t\tName\t\t\tStatus\t\t\tLogin\t\t\tPassword\n\n";
		for (int i = 0; i < users.m_index; ++i)
		{
			os << i << "\t\t" << users.m_users[i].m_name << "\t\t\t";

			if (users.m_users[i].m_role)std::cout << "ADMIN\t\t\t";
			else std::cout << "User\t\t\t";

			os << users.m_users[i].m_login << "\t\t\t" << users.m_users[i].m_password << '\n';
		}
		return os;
	}

	void capacity_up()
	{
		m_size += m_capacity;

		User_data* tmp = new User_data[m_size];
		for (int i = 0; i < m_index; ++i)
		{
			tmp[i] = m_users[i];
		}
		delete[] m_users;
		m_users = tmp;
	}

	int m_size = 5;
	int m_index = 0;
	int m_capacity = 5;
	User_data* m_users = new User_data[m_size];
};

const int count_answer = 4;

struct One_test
{
	friend std::istream& operator>>(std::istream& is, Ekzam& n);
	friend std::ostream& operator<<(std::ostream& os, Ekzam& n);
	std::string m_question;
	std::string m_answer[count_answer];
	int m_correct_answer;
};

class Subject_tests
{
public:
	Subject_tests() = default;

	int get_size()const
	{
		return m_size;
	}

	const std::string& get_subject() const
	{
		return m_subject;
	}

	void set_subject(const std::string subject)
	{
		m_subject = subject;
	}

	Subject_tests(const Subject_tests& other)
	{
		m_index = other.m_index;
		m_size = other.m_size;
		m_arr_test = new One_test[m_size];

		for (size_t i = 0; i < m_index; i++)
			m_arr_test[i] = other.m_arr_test[i];
	}

	Subject_tests(Subject_tests&& other)
	{
		m_index = other.m_index;
		m_size = other.m_size;
		m_arr_test = other.m_arr_test;
		other.m_arr_test = nullptr;
	}

	Subject_tests& operator=(const Subject_tests& other)
	{
		m_index = other.m_index;
		m_size = other.m_size;
		m_arr_test = new One_test[m_size];

		for (size_t i = 0; i < m_index; i++)
			m_arr_test[i] = other.m_arr_test[i];

		return *this;
	}

	Subject_tests& operator=(Subject_tests&& other)
	{
		m_index = other.m_index;
		m_size = other.m_size;
		m_arr_test = other.m_arr_test;
		other.m_arr_test = nullptr;

		return *this;
	}


	~Subject_tests()
	{
		delete[] m_arr_test;
	}

private:

	void capacity_up()
	{
		while (m_index > m_size)
		{
			m_size += m_capacity;
		}

		One_test* tmp = new One_test[m_size];
		for (int i = 0; i < m_index; ++i)
			tmp[i] = m_arr_test[i];

		delete[] m_arr_test;
		m_arr_test = tmp;
	}

	int m_size = 6;
	int m_index = 0;
	int m_capacity = 5;
	friend std::ostream& operator<<(std::ostream& os, Ekzam& n);
	friend std::istream& operator>>(std::istream& is, Ekzam& n);
	friend class Ekzam;
	One_test* m_arr_test = new One_test[m_size];
	std::string m_subject;


	friend std::istream& operator>>(std::istream& is, Subject_tests& n)
	{
		std::cout << "\nEnter subject name\t: ";
		std::getline(is, n.m_subject);
		std::cout << "\n";
		for (int a = 0; a < n.m_size; ++a)
		{
			std::cout << a + 1 << ". " << "Question : ";
			std::getline(is, n.m_arr_test[a].m_question);

			for (int b = 0; b < count_answer; ++b)
			{
				std::cout << b << ") ";
				std::getline(is, n.m_arr_test[a].m_answer[b]);
			}
			std::cout << "Enter correctly answer\t: ";
			is >> n.m_arr_test[a].m_correct_answer;
			std::cin.ignore();
			std::cout << "\n";
		}
		return is;
	}

	friend std::ostream& operator<<(std::ostream& os, Subject_tests& n)
	{
		std::cout << "\n--------------------------------------\nSubject name\t:\t" << n.m_subject;
		std::cout << "\nCount question = " << n.m_size << "\n--------------------------------------\n";

		for (int a = 0; a < n.m_size; ++a)
		{
			std::cout << "\n\n\n------------------------------------\n" << a + 1 << ". " << n.m_arr_test[a].m_question
				<< " ?\n";
			for (int b = 0; b < count_answer; ++b)
			{
				std::cout << '\n' << b << ") " << n.m_arr_test[a].m_answer[b];
			}
		}
		return os;
	}
};

class Ekzam
{
public:
	Ekzam() = default;

	int get_size()const
	{
		return m_index;
	}

	void print_subject()
	{
		for (int i = 0; i < m_index; ++i)
		{
			std::cout << i << ". " << m_arr_subject[i].m_subject << '\n';
		}
	}

	const std::string& get_test_subject(const int index)const
	{
		return m_arr_subject[index].get_subject();
	}

	int get_test_index(const int index)const
	{
		return m_arr_subject[index].get_size();
	}

	int test(const int subject_index, const int test_index)const
	{
		return m_arr_subject[subject_index].m_arr_test[test_index].m_correct_answer;
	}

	Ekzam(const Ekzam& other)
	{
		m_index = other.m_index;
		m_size = other.m_size;
		m_arr_subject = new Subject_tests[m_size];

		for (size_t i = 0; i < m_index; i++)
			m_arr_subject[i] = other.m_arr_subject[i];
	}

	Ekzam(Ekzam&& other)
	{
		m_index = other.m_index;
		m_size = other.m_size;
		m_arr_subject = other.m_arr_subject;
		other.m_arr_subject = nullptr;
	}

	Ekzam& operator=(const Ekzam& other)
	{
		m_index = other.m_index;
		m_size = other.m_size;
		m_arr_subject = new Subject_tests[m_size];

		for (size_t i = 0; i < m_index; i++)
			m_arr_subject[i] = other.m_arr_subject[i];

		return *this;
	}

	Ekzam& operator=(Ekzam&& other)
	{
		m_index = other.m_index;
		m_size = other.m_size;
		m_arr_subject = other.m_arr_subject;
		other.m_arr_subject = nullptr;

		return *this;
	}

	~Ekzam()
	{
		delete[] m_arr_subject;
	}

	void print_testing(const int index)
	{
		std::cout << "\n----------------------------------\nSubject\t: ";
		std::cout << m_arr_subject[index].m_subject;
		std::cout << "\n----------------------------------\n\n";
		for (int b = 0; b < m_arr_subject[index].m_size; ++b)
		{
			std::cout << b + 1 << ". ";
			std::cout << m_arr_subject[index].m_arr_test[b].m_question << "\n\n";
			for (int c = 0; c < count_answer; ++c)
			{
				std::cout << c << ") " << m_arr_subject[index].m_arr_test[b].m_answer[c] << ".   ";
			}
			std::cout << "\n----------------------------------\n";
		}

	}

private:

	void capacity_up()
	{
		while (m_index > m_size)
		{
			m_size += m_capacity;
		}

		Subject_tests* tmp = new Subject_tests[m_size];
		for (int i = 0; i < m_index; ++i)
			tmp[i] = m_arr_subject[i];

		delete[] m_arr_subject;
		m_arr_subject = tmp;
	}

	int m_size = 10;
	int m_index = 0;
	int m_capacity = 5;

	Subject_tests* m_arr_subject = new Subject_tests[m_size];

	friend std::istream& operator>>(std::istream& is, Ekzam& n)
	{
		std::cout << "Enter count subject\t: ";
		is >> n.m_index;
		std::cin.ignore();
		for (int a = 0; a < n.m_index; ++a)
		{
			std::cout << "\n----------------------------------\nSubject\t: ";
			std::getline(is, n.m_arr_subject[a].m_subject);
			std::cout << "----------------------------------\n\n";
			for (int b = 0; b < n.m_arr_subject[a].m_size; ++b)
			{
				std::cout << b + 1 << ". " << "Question : ";
				std::getline(is, n.m_arr_subject[a].m_arr_test[b].m_question);
				for (int c = 0; c < count_answer; ++c)
				{
					std::cout << c << ") ";
					std::getline(is, n.m_arr_subject[a].m_arr_test[b].m_answer[c]);
				}
				std::cout << "Enter correctly answer\t: ";
				is >> n.m_arr_subject[a].m_arr_test[b].m_correct_answer;
				std::cin.ignore();
				std::cout << "\n";
			}
		}
		return is;
	}

	friend std::ostream& operator<<(std::ostream& os, Ekzam& n)
	{
		std::cout << "Count subject\t: ";
		os << n.m_index;
		for (int a = 0; a < n.m_index; ++a)
		{
			std::cout << "\n----------------------------------\nSubject\t: ";
			os << n.m_arr_subject[a].m_subject;
			os << "\n----------------------------------\n\n";
			for (int b = 0; b < n.m_arr_subject[a].m_size; ++b)
			{
				std::cout << b + 1 << ". ";
				os << n.m_arr_subject[a].m_arr_test[b].m_question << "\n\n";
				for (int c = 0; c < count_answer; ++c)
				{
					os << c << ") " << n.m_arr_subject[a].m_arr_test[b].m_answer[c] << ".   ";
				}
				std::cout << "\n----------------------------------\n";
			}
		}
		return os;
	}
};

enum
{
	COME_BACK = -1, EXIT, PASS_TEST, SIGN_UP = 1, SIGN_IN, CREATE_TEST,
	PRINT_USERS = 1, CREATE_USER, DELETE_USER, CREAT_TESTS, PASS_ADMIN
};

int main()
{

	int action, act, sum_count = 0, count = 0, index;
	Ekzam test;
	Users users;
	std::string login;
	std::string password;
	User_data sign_up, user;

	while (true)
	{
		std::cout << "\n\n<< Fill in the registration Admin >> \n\n";
		std::cin >> sign_up;
		system("cls");

		if (sign_up.m_password == sign_up.m_repeat_password)
		{
			sign_up.set_role(User_data::ADMIN);
			users.add(sign_up);
			break;
		}
		else
		{
			std::cout << "Password mismatch\n";
		}
	}

	do
	{
		std::cout << R"(
0 - Exit
1 - Sign up 
2 - Sign in
)";
		std::cin >> action;
		system("cls");
		std::cin.ignore();
		if (action == SIGN_UP)
		{
			std::cout << "\n\n<< Fill in the registration User >> \n\n";
			std::cin >> sign_up;

			if (sign_up.m_password == sign_up.m_repeat_password)
			{
				if (users.add(sign_up))
				{
					std::cout << "\nRegistration was successful\n";
				}
				else
				{
					std::cout << "A user with such data exists\n";
				}
			}
			else
			{
				std::cout << "Password mismatch\n";
			}
		}
		else if (action == SIGN_IN)
		{
			std::cout << "\nEnter data for Sign in\n";
			std::cout << "Login     :  ";
			std::getline(std::cin, login);

			std::cout << "Password  :  ";
			std::getline(std::cin, password);
			system("cls");

			if (users.authentication(login, password))
			{
				try
				{
					sign_up = users.get_user(login, password);
				}
				catch (std::invalid_argument& ex)
				{
					std::cout << ex.what();
				}
				std::cout << "Hello " << sign_up.m_name << std::endl;
				if (!sign_up.m_role)
				{
					do
					{
						std::cout << R"(
-1 - Come back
 0 - Exit
 1 - Pass the test
)";
						std::cin >> action;
						system("cls");
						if (action == COME_BACK || action == EXIT)
						{
							break;
						}
						else if (action == PASS_TEST)
						{
							if (test.get_size())
							{
								std::cout << test;
								std::cout << "\n<<";
								test.print_subject();
								std::cout << " >>\n";

								std::cout << "Please choose subject for test\n";

								std::cin >> index;;
								system("cls");
								std::cout << "\n\nYou select :  test on " << test.get_test_subject(index) << "\n";
								int size = test.get_test_index(index);

								test.print_testing(index);
								for (int j = 0; j < size; ++j)
								{
									std::cout << j + 1 << ") ";
									std::cin >> act;
									sum_count++;
									if (test.test(index, j) == act)
									{
										count++;
									}
								}
								std::cout << "\nNumber of correct answers    : " << count << '\n';
								std::cout << "\nNumber of incorrect answers  : " << sum_count - count << '\n';
							}
							else
							{
								std::cout << "\nNot test!\n\n";
							}
						}
					} while (true);
				}
				else
				{
					while (true)
					{
						std::cout << "\n\t<<  ADMIN  >>\n\n-1 - Come back\n";
						std::cout << " 0 - Exit\n";
						std::cout << " 1 - Print users\n";
						std::cout << " 2 - Creat user\n";
						std::cout << " 3 - Delete user\n";
						std::cout << " 4 - Creat test\n";
						std::cout << " 5 - Pass admin\n";

						std::cin >> action;
						system("cls");
						if (action == PRINT_USERS)
						{
							std::cout << users;
						}
						else if (action == EXIT || action == COME_BACK)
						{
							break;
						}
						else if (action == CREATE_USER) {
							std::cout << "\n\n<< Fill in the registration User >> \n\n";
							std::cin >> sign_up;

							if (sign_up.m_password == sign_up.m_repeat_password)
							{
								if (users.add(sign_up))
								{
									std::cout << "\nRegistration was successful\n";
								}
								else
								{
									std::cout << "A user with such data exists\n";
								}
							}
							else
							{
								std::cout << "Password mismatch\n";
							}
						}
						else if (action == DELETE_USER)
						{
							std::cout << users << '\n';;
							std::cout << "Enter number for delete user\n";
							std::cin >> action;
							if (users.get_role(action) == User_data::ADMIN)
							{
								std::cout << "Cannot remove admin\n";
							}
							else
							{
								try
								{
									users.deleted(action);
								}
								catch (std::range_error& ex)
								{
									std::cout << ex.what();
								}
							}
						}
						else if (action == CREAT_TESTS)
						{
							std::cin >> test;
						}
						else if (action == PASS_ADMIN)
						{
							std::cout << users << '\n';
							std::cout << "\nWho pass admin?\n";
							std::cin >> action;
							users.pass_admin(action);
							std::cout << users << '\n';
							break;
						}
						else {
							std::cout << "\nIncorrect choice\n";
						}
					}
				}
			}
			else
			{
				std::cout << "Data entered incorrectly\n";
			}
		}
		else if (action == EXIT)
		{
			break;
		}
		else
		{
			std::cout << "\nIncorrect choice\n";
		}
	} while (action != EXIT);
}

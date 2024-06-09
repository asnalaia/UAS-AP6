#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>

using namespace std;

struct Mahasiswa {
    int nim;
    string nama;
    int umur;
    float* nilai;
    int jumlahNilai;

    Mahasiswa(int jumlahNilai) : jumlahNilai(jumlahNilai) {

        nilai = new float[jumlahNilai];
    }

    ~Mahasiswa() {
        delete[] nilai;
    }
};

class SistemManajemenMahasiswa {
private:
    vector<Mahasiswa*> mahasiswa;

public:
    SistemManajemenMahasiswa() {
        muatMahasiswaDariFile();
    }

    ~SistemManajemenMahasiswa() {
        simpanMahasiswaKeFile();
        for (auto& mhs : mahasiswa) {
            delete mhs; 
        }
    }

    void tambahMahasiswa() {
        int nim, umur, jumlahNilai;
        string nama;

        cout << "Masukkan NIM: ";
        cin >> nim;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        cout << "Masukkan Nama: ";
        getline(cin, nama);

        cout << "Masukkan Umur: ";
        cin >> umur;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        do {
            cout << "Masukkan Jumlah Nilai (1-100): ";
            cin >> jumlahNilai;
            if (cin.fail() || jumlahNilai <= 0 || jumlahNilai > 100) {
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                cout << "Input tidak valid. Silakan masukkan angka antara 1 dan 100." << endl;
            } else {
                break;
            }
        } while (true);

        try {
            Mahasiswa* mhs = new Mahasiswa(jumlahNilai);
            mhs->nim = nim;
            mhs->nama = nama;
            mhs->umur = umur;

            cout << "Masukkan Nilai: ";
            for (int i = 0; i < jumlahNilai; ++i) {
                cin >> mhs->nilai[i];
            }

            mahasiswa.push_back(mhs);
            cout << "Data Mahasiswa berhasil ditambahkan.\n";
        } catch (const std::bad_alloc&) {
            cout << "Gagal mengalokasikan memori untuk nilai. Silakan coba lagi dengan jumlah nilai yang valid.\n";
        }
    }

    void perbaruiMahasiswa() {
        int nim;
        cout << "Masukkan NIM mahasiswa yang ingin diperbarui: ";
        cin >> nim;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        for (auto& mhs : mahasiswa) {
            if (mhs->nim == nim) {
                cout << "Masukkan Nama baru: ";
                getline(cin, mhs->nama);

                cout << "Masukkan Umur baru: ";
                cin >> mhs->umur;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                int jumlahNilai;
                do {
                    cout << "Masukkan Jumlah Nilai baru (1-100): ";
                    cin >> jumlahNilai;
                    if (cin.fail() || jumlahNilai <= 0 || jumlahNilai > 100) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Input tidak valid. Silakan masukkan angka antara 1 dan 100." << endl;
                    } else {
                        break;
                    }
                } while (true);

                delete[] mhs->nilai; 
                try {
                    mhs->nilai = new float[jumlahNilai];
                    mhs->jumlahNilai = jumlahNilai;

                    cout << "Masukkan Nilai baru: ";
                    for (int i = 0; i < jumlahNilai; ++i) {
                        cin >> mhs->nilai[i];
                    }
                    cout << "Data Mahasiswa berhasil diperbarui.\n";
                } catch (const std::bad_alloc&) {
                    cout << "Gagal mengalokasikan memori untuk nilai. Silakan coba lagi dengan jumlah nilai yang valid.\n";
                }
                return;
            }
        }
        cout << "Data Mahasiswa tidak ditemukan.\n";
    }

    void tampilkanMahasiswa() const {
        for (const auto& mhs : mahasiswa) {
            cout << "NIM: " << mhs->nim << ", Nama: " << mhs->nama
                 << ", Umur: " << mhs->umur << ", Nilai: ";
            for (int i = 0; i < mhs->jumlahNilai; ++i) {
                cout << mhs->nilai[i] << " ";
            }
            cout << endl;
        }
    }

    void hapusMahasiswa() {
        int nim;
        cout << "Masukkan NIM mahasiswa yang ingin dihapus: ";
        cin >> nim;

        for (auto it = mahasiswa.begin(); it != mahasiswa.end(); ++it) {
            if ((*it)->nim == nim) {
                delete *it; 
                mahasiswa.erase(it);
                cout << "Data Mahasiswa berhasil dihapus.\n";
                return;
            }
        }
        cout << "Data Mahasiswa tidak ditemukan.\n";
    }

    void cariMahasiswa() const {
        int nim;
        cout << "Masukkan NIM mahasiswa yang ingin dicari: ";
        cin >> nim;

        for (const auto& mhs : mahasiswa) {
            if (mhs->nim == nim) {
                cout << "NIM: " << mhs->nim << ", Nama: " << mhs->nama
                     << ", Umur: " << mhs->umur << ", Nilai: ";
                for (int i = 0; i < mhs->jumlahNilai; ++i) {
                    cout << mhs->nilai[i] << " ";
                }
                cout << endl;
                return;
            }
        }
        cout << "Data Mahasiswa tidak ditemukan.\n";
    }

    void muatMahasiswaDariFile() {
        ifstream file("mahasiswa.txt");
        if (!file) {
            cout << "Tidak ada file data ditemukan. Memulai dengan database kosong." << endl;
            return;
        }

        int jumlahMahasiswa;
        file >> jumlahMahasiswa;
        for (int i = 0; i < jumlahMahasiswa; ++i) {
            int nim, umur, jumlahNilai;
            string nama;
            file >> nim >> nama >> umur >> jumlahNilai;

            Mahasiswa* mhs = new Mahasiswa(jumlahNilai);
            mhs->nim = nim;
            mhs->nama = nama;
            mhs->umur = umur;

            for (int j = 0; j < jumlahNilai; ++j) {
                file >> mhs->nilai[j];
            }

            mahasiswa.push_back(mhs);
        }

        file.close();
    }

    void simpanMahasiswaKeFile() const {
        ofstream file("mahasiswa.txt");
        file << mahasiswa.size() << endl;
        for (const auto& mhs : mahasiswa) {
            file << mhs->nim << " " << mhs->nama << " " << mhs->umur << " " << mhs->jumlahNilai << " ";
            for (int i = 0; i < mhs->jumlahNilai; ++i) {
                file << mhs->nilai[i] << " ";
            }
            file << endl;
        }

        file.close();
    }
};

int main() {
    SistemManajemenMahasiswa smm;
    int pilihan;

    do {
        cout << "\nSistem Manajemen Mahasiswa\n";
        cout << "1. Tambah Data Mahasiswa\n";
        cout << "2. Perbarui Data Mahasiswa\n";
        cout << "3. Tampilkan Data Mahasiswa\n";
        cout << "4. Hapus Data Mahasiswa\n";
        cout << "5. Cari Data Mahasiswa\n";
        cout << "6. Keluar\n";
        cout << "Masukkan pilihan Anda: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                smm.tambahMahasiswa();
                break;
            case 2:
                smm.perbaruiMahasiswa();
                break;
            case 3:
                smm.tampilkanMahasiswa();
                break;
            case 4:
                smm.hapusMahasiswa();
                break;
            case 5:
                smm.cariMahasiswa();
                break;
            case 6:
                cout << "Keluar...\n";
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi.\n";
        }
    } while (pilihan != 6);

    return 0; 
}

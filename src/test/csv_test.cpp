//
// Created by kiki on 2018/07/19.
//


#include <iostream>
#include "../core/csv.h"

int main()
{
    std::string filename = "../property/SiO2.csv";
    std::vector<double> spectrum;

    io::CSVReader<2> in(filename);
    in.read_header(io::ignore_extra_column, "Wavelength", "Intensity");
    int wave;
    double intensity;
    std::vector<std::tuple<int, double>> spectrumData;

    // 波長データを取得
    while (in.read_row(wave, intensity)) {
//        std::cout << "wave, intensity : " << wave << ", " << intensity << std::endl;
        spectrumData.emplace_back(wave, intensity);
    }

    // データが一つの場合はすべての波長をその値に置き換える
    // データは380~780のカバーしていると仮定している
    if (spectrumData.size() == 1) {
        for (int i = 0; i < 401; i++) {
            spectrum.push_back(std::get<1>(spectrumData[0]));
        }
    } else if (spectrumData.size() > 1) {
        int diff = std::get<0>(spectrumData[1]) - std::get<0>(spectrumData[0]);
//        std::cout << "diff : " << diff << std::endl;

        // resolutionが1の場合は380~780すべてをカバーしていると仮定して
        // csvファイルからデータを読み取る
        if (diff == 1) {
            auto lastSpec = static_cast<int>(std::get<0>(spectrumData[0]) + (spectrumData.size() - 1) * diff);
//            std::cout << "lastSpec : " << lastSpec << std::endl;

            // 380nmのインデックスを調べる
            int index = 0;
            for (int i = 0; i < spectrumData.size(); ++i) {
                if (std::get<0>(spectrumData[i]) == 380) {
                    index = i;
                    break;
                }
            }

            for (int i = 0; i < 401; i++) {
                spectrum.push_back(std::get<1>(spectrumData[i + index]));
            }

            std::cout << "-------------" << std::endl;
            std::cout << "spectrum data size : " << spectrum.size() << std::endl;
            for (int i = 0; i < spectrum.size(); i++) {
                std::cout << i + 380 << " : " << spectrum[i] << std::endl;
            }
        } else if (diff == 5) {
            // 380nmのインデックスを調べる
            int index = 0;
            for (int i = 0; i < spectrumData.size(); ++i) {
                if (std::get<0>(spectrumData[i]) == 380) {
                    index = i;
                    break;
                }
            }

//            std::cout << "index : " << index << std::endl;
            for (int i = 0; i < 401; i++) {
                double rate = (380 + i) % 5;
                if (rate == 0) {
                    spectrum.push_back(std::get<1>(spectrumData[(i / 5) + index]));
//                    printf("%d : %f\n", 380 + i, std::get<1>(spectrumData[(i / 5) + index]));
                } else {
                    int nowSpec = i / 5;
                    auto a = std::get<1>(spectrumData[(nowSpec) + index]);
                    auto b = std::get<1>(spectrumData[(nowSpec + 1) + index]);
                    double spec = (1.0 - rate / 5.0) * a + (rate / 5.0) * b;
//                    printf("%d : %f\t --- (%.5f, %.5f) \t --- rate : %3f\t --\n", 380 + i, spec, a, b, rate);
                    spectrum.push_back(spec);
                }


            }

            std::cout << "-------------" << std::endl;
            std::cout << "spectrum data size : " << spectrum.size() << std::endl;
            for (int i = 0; i < spectrum.size(); i++) {
                printf("%d : %5f\n", i + 380, spectrum[i]);
            }
        }
    } else {
        std::cerr << "スペクトルデータがありません : "
                  << filename << std::endl;
    }
}
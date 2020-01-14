#include "SSA.h"

int best_iteration;
bool achou_cobertura_menor;

Position Position::init_position(Problem* problem) {
    std::vector<double> solution(problem->dimension);
    for (int i = 0; i < problem->dimension; ++i) {
        solution[i] = randu();
    }
    return Position(solution, 1E100);
}

Vibration::Vibration(const Position& position) :
        intensity(fitness_to_intensity(position.fitness)), position(position) {

}

Vibration::Vibration(double intensity, const Position& position) :
        intensity(intensity), position(position) {

}

double Vibration::intensity_attenuation(double attenuation_factor, double distance) const {
    return intensity * exp(-distance / attenuation_factor);
}

double Vibration::fitness_to_intensity(double fitness) {
    return  log(1.0 / (fitness - C) + 1.0);
}

double Vibration::C = -1E-100;

void Spider::choose_vibration(const std::vector<Vibration>& vibrations,
        const std::vector<double>& distances, double attenuation_factor) {
    int max_index = -1;
    double max_intensity = target_vibr.intensity;
    for (int i = 0; i < vibrations.size(); ++i) {
        if (vibrations[i].position == target_vibr.position) {
            continue;
        }
        double intensity = vibrations[i].intensity_attenuation(attenuation_factor, distances[i]);
        if (intensity > max_intensity) {
            max_index = i;
            max_intensity = intensity;
        }
    }
    if (max_index != -1) {
        target_vibr = Vibration(max_intensity, vibrations[max_index].position);
        inactive_deg = 0;
    } else {
        ++inactive_deg;
    }
}

void Spider::mask_changing(double p_change, double p_mask) {
    if (randu() > pow(p_change, inactive_deg)) {
        inactive_deg = 0;
        p_mask *= randu();
        for (int i = 0; i < dimension_mask.size(); ++i) {
            dimension_mask[i] = (randu()) < p_mask;
        }
    }
}

Spider::Spider(const Position& position) :
        position(position), inactive_deg(0) {
    target_vibr = Vibration(0, position);
    dimension_mask.resize(position.solution.size());
    previous_move.resize(position.solution.size());
}

void Spider::random_walk(const std::vector<Vibration>& vibrations) {
    for (int i = 0; i < position.solution.size(); ++i) {
        previous_move[i] *= randu();
        double target_position = dimension_mask[i] ?
                                vibrations[rand() % vibrations.size()].position.solution[i] :
                                target_vibr.position.solution[i];
        previous_move[i] += randu() * (target_position - position.solution[i]);
        position.solution[i] += previous_move[i];
    }
}

SpiderCVM::SpiderCVM(const Position& position) :
        Spider(position){
}

SpiderGCVM::SpiderGCVM(const Position& position,Problem* problem) :
        Spider(position), problem(problem){
}

void SpiderCVM::random_walk(const std::vector<Vibration>& vibrations) {
    for (int i = 0; i < position.solution.size(); ++i) {
        previous_move[i] *= randu();
        double target_position = dimension_mask[i] ?
                                vibrations[rand() % vibrations.size()].position.solution[i] :
                                target_vibr.position.solution[i];
        double target_move = randu() * (target_position - position.solution[i]);
        previous_move[i] += target_move;
        position.solution[i] += previous_move[i];
        position.solution[i] = std::min(position.solution[i],1.0);
        position.solution[i] = std::max(position.solution[i],0.0);
    }
}

void SpiderGCVM::random_walk(const std::vector<Vibration>& vibrations) {
    int indice_melhor_troca = 0;
    int melhor_solucao = INT32_MAX;
    int solucao_atual,position_atual;

    //Realiza o movimento do GCVM, trocando apenas um vertice
    if(rand() % 2 == 0){
        //Calcula o melhor vertice a ser trocado
        for (int i = 0; i < position.solution.size(); ++i) {
            position_atual = position.solution[i];
            //Troca o valor do vertice e testa o valor da nova cobertura
            if(position.solution[i]>=0.5){
                position.solution[i] = 0;
                solucao_atual = problem->eval(position.solution);
                if(solucao_atual<melhor_solucao){
                    melhor_solucao = solucao_atual;
                    indice_melhor_troca = i;
                }
            }else{
                position.solution[i] = 1;
                solucao_atual = problem->eval(position.solution);
                if(solucao_atual<melhor_solucao){
                    melhor_solucao = solucao_atual;
                    indice_melhor_troca = i;
                }
            }
            position.solution[i] = position_atual;
        }
        if(position.solution[indice_melhor_troca] == 0){
            position.solution[indice_melhor_troca] = 1;
        }else{
            position.solution[indice_melhor_troca] = 0;
        }
    }
    //Realiza o movimento padrao das aranhas
    else{  
        for (int i = 0; i < position.solution.size(); ++i) {
            previous_move[i] *= randu();
            double target_position = dimension_mask[i] ?
                                    vibrations[rand() % vibrations.size()].position.solution[i] :
                                    target_vibr.position.solution[i];
            previous_move[i] += randu() * (target_position - position.solution[i]);
            position.solution[i] += previous_move[i];
            position.solution[i] = std::min(position.solution[i],1.0);
            position.solution[i] = std::max(position.solution[i],0.0);
        }
    }
}


SSA::SSA(Problem* problem, unsigned int pop_size) :
        problem(problem), dimension(problem->dimension) {
    srand((unsigned int)(time(NULL) + (int)this));
    population.reserve(pop_size);
    distances.resize(pop_size);
    for (int i = 0; i < pop_size; ++i) {
        Position position = Position::init_position(problem);
        // population.push_back(new SpiderCVM(position));
        population.push_back(new SpiderGCVM(position,problem));
        distances[i].resize(pop_size);
    }
    vibrations.reserve(pop_size);
    global_best = population[0]->position;
}

void SSA::run(int max_iteration, double attenuation_rate, double p_change, double p_mask) {
    print_header();
    start_time = std::chrono::high_resolution_clock::now();
    for (iteration = 1; iteration <= max_iteration; ++iteration) {
        achou_cobertura_menor = false;
        fitness_calculation();
        vibration_generation(attenuation_rate);
        for (int i = 0; i < population.size(); ++i) {
            population[i]->mask_changing(p_change, p_mask);
            population[i]->random_walk(vibrations);
        }
        if ((iteration == 1) || (iteration == 10)
                || (iteration < 1001 && iteration % 100 == 0)
                || (iteration < 10001 && iteration % 1000 == 0)
                || (iteration < 100001 && iteration % 10000 == 0)
                || achou_cobertura_menor) {
            print_content();
        }
    }
    iteration--;
    print_footer();
}

void SSA::fitness_calculation() {
    population_best_fitness = 1E100;
    for (int i = 0; i < population.size(); ++i) {
        double fitness = problem->eval(population[i]->position.solution);
        population[i]->position.fitness = fitness;
        if (fitness < global_best.fitness) {
            global_best = population[i]->position;
            best_iteration = iteration;
            achou_cobertura_menor = true;
        }
        if (fitness < population_best_fitness) {
            population_best_fitness = fitness;
        }
    }
    mean_distance = 0;
    for (int i = 0; i < population.size(); ++i) {
        for (int j = i + 1; j < population.size(); ++j) {
            distances[i][j] = population[i]->position - population[j]->position;
            distances[j][i] = distances[i][j];
            mean_distance += distances[i][j];
        }
    }
    mean_distance /= (population.size() * (population.size() - 1) / 2);
}

void SSA::vibration_generation(double attenuation_rate) {
    vibrations.clear();
    for (int i = 0; i < population.size(); ++i) {
        vibrations.push_back(std::move(Vibration(population[i]->position)));
    }
    double sum = 0.0;
    std::vector<double> data;
    data.resize(population.size());
    for (int i = 0; i < dimension; ++i) {
        for (int j = 0; j < population.size(); ++j) {
            data[j] = population[j]->position.solution[i];
        }
        sum += std_dev(data);
    }
    attenuation_base = sum / dimension;
    for (int i = 0; i < population.size(); ++i) {
        population[i]->choose_vibration(vibrations, distances[i], attenuation_base * attenuation_rate);
    }
}

void SSA::print_header() {
    std::cout << "               SSA starts at "
            << get_time_string() << std::endl
            << "==============================================================" << std::endl
            << " iter    optimum    pop_min  base_dist  mean_dist time_elapsed" << std::endl
            << "==============================================================" << std::endl;
}

void SSA::print_content() {
    auto current_time = std::chrono::high_resolution_clock::now();
    printf("%5d %.3e %.3e %.3e %.3e ", iteration, global_best.fitness, population_best_fitness,
            attenuation_base, mean_distance);
    std::cout <<get_time_string(std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time)) << std::endl;
}

void SSA::print_footer() {
    std::cout << "==============================================================" << std::endl;
    print_content();
    std::cout << "==============================================================" << std::endl;

    printf("Respostas: \n");
    for(int i = 0; i < global_best.solution.size(); i++ )
        printf("%d -> %.2lf  ", i+1, global_best.solution[i]);
    std::cout << "\n==============================================================" << std::endl;

    printf("Qtd minima de iteracoes para encontrar a cobertura: %d\n\n",best_iteration);

    printf("Vertices da Cobertura: \n");
    int total = 0;
    for(int i = 0; i < global_best.solution.size(); i++ ){
        if(global_best.solution[i] >= 0.5){
            printf("%d ", i+1 );
            total++;
        }
    }
    printf("\n\nTotal: %d",total);
    if(total!=(int)global_best.fitness){
        printf("\nO CONJUNTO ACIMA NAO E' UMA COBERTURA: Total de vertices != Funcao Fitness (%d  !=  %d)\nCOBERTURA NAO ENCONTRADA!",total,(int)global_best.fitness);
    }
    std::cout << "\n==============================================================" << std::endl;
}

std::string get_time_string() {
    using namespace std::chrono;

    auto time_t = high_resolution_clock::to_time_t(high_resolution_clock::now());
    auto ttm = localtime(&time_t);

    char time_str[80];
    strftime(time_str, 80, "%Y-%m-%d %H:%M:%S", ttm);
    return std::string(time_str);
}

std::string get_time_string(std::chrono::milliseconds ms) {
    long long int mss = ms.count();
    char time_str[80];
    sprintf(time_str, "%02d:%02d:%02d.%03d", int(mss / 3600000), int(mss / 60000 % 60),
            int(mss / 1000 % 60), int(mss % 1000));
    return std::string(time_str);
}
#pragma once

#include "Option.hpp"
#include "BlackScholesModel.hpp"
#include "pnl/pnl_random.h"
#include "parser.hpp"
#include <mpi.h>

/*!
 * \file MonteCarlo.hpp
 * \brief Simulation de Monte-Carlo
 */
class MonteCarlo
{
public:
    BlackScholesModel *mod_; /*! pointeur vers le modèle */
    Option *opt_; /*! pointeur sur l'option */
    PnlRng *rng_; /*! pointeur sur le générateur */
    double fdStep_; /*! pas de différence finie */
    size_t nbSamples_; /*! nombre de tirages Monte Carlo */
    
    PnlMat *shiftPlus_;
    PnlMat *shiftMoins_;
    PnlMat *path_;

    /**
     * \brief Constructeur par defaut
     */
    MonteCarlo();
    
    /**
     * \brief Constructeur à partir d'un fichier
     * @param P  parseur d'extraction des données dans un fichier
     */
    MonteCarlo(Param *P);
    
    
        /**
     * \brief Constructeur à partir d'un fichier
     * @param P  parseur d'extraction des données dans un fichier
     */
    MonteCarlo(Param *P, PnlRng *rng, int size);
    
    /**
     * \brief Destructeur
     */
    ~MonteCarlo();
    
    /**
     * Calcule le prix de l'option à la date 0
     *
     * @param[out] prix valeur de l'estimateur Monte Carlo
     * @param[out] ic largeur de l'intervalle de confiance
     */
    void price(double &prix, double &ic);
    void price_parallelisation(double &variance, int size, int rank, double &prix, double &ic, bool cond, int nb_tirages_previous, int nb_tirages, double &memorized_sum, double &memorized_sum_square);
    
    void price_slave(double &sum, double &sum_square, int size, int rank);
    void monte_carlo_slave(double &sum, double &sum_square, int size, int nb_tirages); 
    void monte_carlo_master(double &prix, double &ic, double sum, double sum_square, double &variance, int nb_tirages);
    void price_master(double &prix, double &ic, double sum, double sum_square, double &variance);
    void monte_carlo_parallele(double &sum, double &sum_square, int size, int rank, int nb_tirages); 
 
    double getVarianceNbTirages(double sum, double sum_square, double t, int nb_tirages);
    double getPriceNbTirages(double sum, double t, int nbTirages);
    double getIntervalleConfianceNbTirages(double variance, int nbTirages);



    
    /**
     * Calcule le prix de l'option à la date 0 pour les tests
     *
     * @param[out] prix valeur de l'estimateur Monte Carlo
     * @param[out] ic largeur de l'intervalle de confiance
     * @param[in] inVects tableau de pointeurs sur des vecteurs G
     * @param[in] size taille du tableau inVects
     */
    void price(double &prix, double &ic, PnlVect *G);

    /**
     * Calcule le prix de l'option à la date t
     *
     * @param[in]  past contient la trajectoire du sous-jacent
     * jusqu'à l'instant t
     * @param[in] t date à laquelle le calcul est fait
     * @param[out] prix contient le prix
     * @param[out] ic contient la largeur de l'intervalle
     * de confiance sur le calcul du prix
     */
    void price(const PnlMat *past, double t, double &prix, double &ic);
    
    /**
     * Calcule le prix de l'option à la date t pour les tests
     *
     * @param[in]  past contient la trajectoire du sous-jacent
     * jusqu'à l'instant t
     * @param[in] t date à laquelle le calcul est fait
     * @param[out] prix contient le prix
     * @param[out] ic contient la largeur de l'intervalle
     * de confiance sur le calcul du prix
     */
    void price(const PnlMat *past, double t, double &prix, double &ic, PnlVect *G);
    
    /**
     * 
     * @param sum pour le calcul de la deuxième partie de la variance
     * @param sum_square pour le calcul de la première partie de la variance
     * @return double la variance
     */
    double getVariance(double sum, double sum_square, double t);
    
    /**
     * @param sum
     * @return double the price of the option
     */
    double getPrice(double sum, double t);
    
    /**
     * Fonction necessaire pour le calcul de l'intervalle de confiance
     *  
     * @param variance
     * @return double the width of the confidence interval
     */
    double getIntervalleConfiance(double variance);

    /**
     * Calcule le delta de l'option à la date t
     *
     * @param[in] past contient la trajectoire du sous-jacent
     * jusqu'à l'instant t
     * @param[in] t date à laquelle le calcul est fait
     * @param[out] delta contient le vecteur de delta
     * de confiance sur le calcul du delta
     */
    void delta(const PnlMat *past, double t, PnlVect *delta);
    
    /**
     * Calcule le delta de l'option à la date t
     * Fonction utilisée pour les tests 
     * 
     * @param[in] past  contient la trajectoire du sous-jacent
     * jusqu'à l'instant t
     * @param[in] t  date à laquelle le calcul est fait
     * @param[out] delta  contient le vecteur de delta
     * de confiance sur le calcul du delta
     * @param[in] vect  vecteur représentant l'aléatoire du modèle
     */
    void delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *vect);
    
    
};



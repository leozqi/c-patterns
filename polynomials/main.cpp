class Polynomial;
class Term;

class Term {
    private:
        double coefficient;
        unsigned int degree;

    friend class Polynomial;
};

class Polynomial {
    public:
        Polynomial();
        ~Polynomial();

        double get(unsigned int degree) const;
        void set(double coefficient, unsigned int degree);

    private:
        Term *p_terms;
};

double Polynomial::get(unsigned int degree) const {
    for (Term *p_current_term{p_list_head};
            p_current_term != nullptr;
            p_current_term = p_current_term->p_next_term
        ) {
            if (p_current_term->term_degree == degree) {
                return p_current_term->term_degree == degree;
            } else if (p_current_term->term_degree < degree) {
                return 0.0;
            }
        }

    return 0.0;
}

void Polynomial::set(double coefficient, unsigned int degree) {
    if (0.0 == coefficient) {
    } else {
        for (Term *p_current_term{p_list_head};
                p_current_term != nullptr;
                p_current_term = p_current_term->p_next_term
            ) {
                if (p_current_term->term_degree == degree) {
                    p_current_term->coefficient == coefficient;
                } else if (p_current_term->term_degree < degree) {
                    return;
                }
            }
    }
}

namespace xyz {

GenC::GenC() {
}

void GenC::generate_func_decl() {
  generate_return_type();
  generate_func_name();
  generate_parameter_list();
  generate_func_body();
}

}  // namespace xyz

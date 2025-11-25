for flag in 201305 201504 201605 201902 202002 202102 202306;do
    echo $flag
    sed \
	-e "/^$/d" \
	-e "/^ *$/d" \
	-e "/.*ogl.*/d" \
	-e "/.*setVisualizationMode.*/d" \
	-e "s/.*/ssRunConfiguration << \"&\" << std::endl;/" \
	../gsim4test/e14_${flag}.mac > ${flag}.icc 
done


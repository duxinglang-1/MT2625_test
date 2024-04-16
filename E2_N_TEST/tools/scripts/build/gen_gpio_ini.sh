#!/bin/bash
echo -e '[GPIO_SETTING]\r\n' > $3
while read line
do
	if [[ $line == *=* ]]
	then
		#echo $line
		lvalue=`echo $line|awk -F '=' '{print $1}'`
		rvalue=`echo $line|awk -F '=' '{print $2}'`
		#echo ~~$lvalue
		#echo ~~$rvalue
		if [[ $lvalue == *gnss_32k_clock* ]]
		then
			if [[ $rvalue == *HAL_GPIO_CLOCK_* ]]
			then
				#echo ~~helloprint $lvalue=$rvalue
				rvalue=`echo ${rvalue%;*}`
				rvalue=`echo ${rvalue##*_}`
				echo -e "GNSS_32K_MODE=$rvalue\r\n" >>$3
			fi
		elif [[ $lvalue == *gnss_clock_pin* ]]
		then
			if [[ $rvalue == *HAL_GPIO_* ]]
			then
				#echo ~~helloprint $lvalue=$rvalue
				rvalue=`echo ${rvalue%;*}`
				rvalue=`echo ${rvalue##*_}`
				echo -e "GNSS_32K_PIN=$rvalue\r\n" >>$3
			fi
		elif [[ $lvalue == *gnss_uart* ]]
		then
			if [[ $rvalue == *HAL_UART_* ]]
			then
				#echo ~~helloprint $lvalue=$rvalue
				rvalue=`echo ${rvalue%;*}`
				rvalue=`echo ${rvalue##*_}`
				uart_index=$rvalue
				echo -e "GNSS_UART_INDEX=$rvalue\r\n" >>$3
			fi
		fi
	fi
done < $1

while read line
do
	if [[ $line == *=* ]]
	then
		#echo $line
		lvalue=`echo $line|awk -F '=' '{print $1}'`
		rvalue=`echo $line|awk -F '=' '{print $2}'`
		#echo ~~$lvalue
		#echo ~~$rvalue
		if [[ $line == *HAL_UART${uart_index}'_RXD_PIN ='* ]]
		then
			#echo ~~helloprint $lvalue=$rvalue
			rvalue=`echo ${rvalue%;*}`
			rvalue=`echo ${rvalue##* }`
			echo -e "GNSS_UART_RX_PIN=$rvalue\r\n" >>$3
		elif [[ $line == *HAL_UART${uart_index}_RXD_PIN_M_UART${uart_index}'_RXD ='* ]]
		then
			#echo ~~helloprint $lvalue=$rvalue
			rvalue=`echo ${rvalue%;*}`
			rvalue=`echo ${rvalue##* }`
			echo -e "GNSS_UART_RX_MODE=$rvalue\r\n" >>$3
		elif [[ $line == *HAL_UART${uart_index}_TXD_PIN' ='* ]]
		then
				#echo ~~helloprint $lvalue=$rvalue
				rvalue=`echo ${rvalue%;*}`
				rvalue=`echo ${rvalue##* }`
				echo -e "GNSS_UART_TX_PIN=$rvalue\r\n" >>$3
		elif [[ $line == *HAL_UART${uart_index}_TXD_PIN_M_UART${uart_index}'_TXD ='* ]]
		then
			#echo ~~helloprint $lvalue=$rvalue
			rvalue=`echo ${rvalue%;*}`
			rvalue=`echo ${rvalue##* }`
			echo -e "GNSS_UART_TX_MODE=$rvalue\r\n" >>$3
		elif [[ $line == *'BSP_GNSS_POWER_PIN ='* ]]
		then
			#echo ~~helloprint $lvalue=$rvalue
			rvalue=`echo ${rvalue%;*}`
			rvalue=`echo ${rvalue##* }`
			echo -e "GNSS_POWER_PIN=$rvalue\r\n" >>$3
		fi
	fi
done < $2

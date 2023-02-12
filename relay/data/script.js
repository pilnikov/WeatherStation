			let nIntervId;
			let req_done = true;

			function loadVal_WiFi()
			{
				let url = '/jwifi';

				if (req_done)
				{
					req_done = false;
					var xh = new XMLHttpRequest();
					xh.open('GET', url, true);
					xh.send(null);
					xh.onreadystatechange = function()
					{
						if (this.readyState == 4 && this.status == 200) 
						{
							var res = JSON.parse(xh.responseText);
							document.getElementById('aps' ).value   = res.as;
							document.getElementById('app' ).value   = res.ap;
							document.getElementById('stas1').value  = res.ss1;
							document.getElementById('stap1').value  = res.sp1;
							document.getElementById('stas2').value  = res.ss2;
							document.getElementById('stap2').value  = res.sp2;
							req_done = true;
							loadVal_Par();
						}
					};
				}
			}
	  
			function loadVal_Par()
			{
				let url = '/jpar';

				if (req_done)
				{
					req_done = false;
					var xh = new XMLHttpRequest();
					xh.open('GET', url, true);
					xh.send(null);
					xh.onreadystatechange = function()
					{
						if (this.readyState == 4 && this.status == 200) 
						{
							var res = JSON.parse(xh.responseText);
							document.getElementById('ch1_gpio').value = res.ch1_gpio;
							document.getElementById('ch2_gpio').value = res.ch2_gpio;
							if (res.ch1_type) document.getElementById('ch1_in').value = 'Input';
							if (!res.ch1_type) document.getElementById('ch1_in').value = 'Output';
							if (res.ch2_type) document.getElementById('ch2_in').value = 'Input';
							if (!res.ch2_type) document.getElementById('ch2_in').value = 'Output';
							if (res.ch1_dig) document.getElementById('ch1_dig').value = 'Digital';
							if (!res.ch1_dig) document.getElementById('ch1_dig').value = 'Analog';
							if (res.ch2_dig) document.getElementById('ch2_dig').value = 'Digital';
							if (!res.ch2_dig) document.getElementById('ch2_dig').value = 'Analog';
							document.getElementById('ch1_on_code').value = res.ch1_on_code;
							document.getElementById('ch1_off_code').value = res.ch1_off_code;
							document.getElementById('ch2_on_code').value = res.ch2_on_code;
							document.getElementById('ch2_off_code').value = res.ch2_off_code;
							document.getElementById('lim_l').value = res.lim_l;
							document.getElementById('lim_h').value = res.lim_h;
							req_done = true;
						}
					};
				}
			}
							
			function fCh1_on()
			{
				let url = '/ch1_set';
			    send_url(url);
			}

			function fCh1_auto()
			{
				let url = '/ch1_auto';
			    send_url(url);
			}

			function fCh2_on()
			{
				let url = '/ch2_set';
			    send_url(url);
			}

			function fCh2_auto()
			{
				let url = '/ch2_auto';
			    send_url(url);
			}
			
			function fCh1_dig()
			{
				let state = document.getElementById('ch1_dig').value;
				if (state == 'Analog')   document.getElementById('ch1_dig').value = 'Digital';
				if (state == 'Digital')  document.getElementById('ch1_dig').value = 'Analog';
			}

			function fCh2_dig()
			{
				let state = document.getElementById('ch2_dig').value;
				if (state == 'Analog')   document.getElementById('ch2_dig').value = 'Digital';
				if (state == 'Digital')  document.getElementById('ch2_dig').value = 'Analog';
			}

			function fCh1_in()
			{
				let state = document.getElementById('ch1_in').value;
				if (state == 'Output')   document.getElementById('ch1_in').value = 'Input';
				if (state == 'Input')  document.getElementById('ch1_in').value = 'Output';
			}

			function fCh2_in()
			{
				let state = document.getElementById('ch2_in').value;
				if (state == 'Output')   document.getElementById('ch2_in').value = 'Input';
				if (state == 'Input')  document.getElementById('ch2_in').value = 'Output';
			}

			function fCh1_value()
			{
				var ch1_value = document.getElementById('ch1_value').value;
				var url = '/ch1_val?val=' + ch1_value;
			    send_url(url);
			}

			function fCh2_value()
			{
				var ch2_value = document.getElementById('ch2_value').value;
				var url = '/ch2_val?val=' + ch2_value;
			    send_url(url);
			}


			function set_wifi()
			{
				var as  = document.getElementById(  'aps').value;
				var ap  = document.getElementById(  'app').value;
				var ss1 = document.getElementById('stas1').value;
				var sp1 = document.getElementById('stap1').value;
				var ss2 = document.getElementById('stas2').value;
				var sp2 = document.getElementById('stap2').value;

				var st1 = 0;
				var st2 = 0;

				var iap = '192.168.4.1';
				var map = '255.255.255.0';

				var wof = 0;

				let jstr  = {
							  as: as,
							  ap: ap,
							 iap: iap,
							 map: map,
							 ss1: ss1,
							 sp1: sp1,
							 st1: st1,
							 ss2: ss2,
							 sp2: sp2,
							 st2: st2,
							 wof: wof
							};

				var url2Str = '/set_wifi?in=' + JSON.stringify(jstr);
			
			  send_url(url2Str);
			}

			function set_par()
			{
				var ch1_gpio  		= document.getElementById('ch1_gpio').value;
				var ch2_gpio  		= document.getElementById('ch2_gpio').value;
				var ch1_type		= 0;
				if (document.getElementById('ch1_in').value == 'Input') ch1_type = 1;
				var ch2_type		= 0;
				if (document.getElementById('ch2_in').value == 'Input') ch2_type = 1;
				var ch1_dig			= 0;
				if (document.getElementById('ch1_dig').value == 'Digital') ch1_dig = 1;
				var ch2_dig			= 0;
				if (document.getElementById('ch2_dig').value == 'Digital') ch2_dig = 1;
				var ch1_on_code   	= document.getElementById('ch1_on_code').value;
				var ch1_off_code  	= document.getElementById('ch1_off_code').value;
				var ch2_on_code   	= document.getElementById('ch2_on_code').value;
				var ch2_off_code	= document.getElementById('ch2_off_code').value;
				var lim_l      		= document.getElementById('lim_l').value;
				var lim_h      		= document.getElementById('lim_h').value;

				var jstr = {
                        ch1_gpio	: ch1_gpio,
                        ch1_type	: ch1_type,
                        ch1_dig		: ch1_dig,
                        ch1_on_code : ch1_on_code,
                        ch1_off_code: ch1_off_code,
                        ch2_gpio	: ch2_gpio,
                        ch2_type	: ch2_type,
                        ch2_dig 	: ch2_dig,
                        ch2_on_code : ch2_on_code,
                        ch2_off_code: ch2_off_code,
                        lim_l    	: lim_l,
                        lim_h    	: lim_h
                      };

				var url2Str = '/set_par?in=' + JSON.stringify(jstr);
			
			  send_url(url2Str);
			}
			
			function send_url(url)
			{
				if (req_done)
				{
					req_done = false;
					var xh = new XMLHttpRequest();
					xh.open('GET', url, true);
					xh.send(null);
					xh.onreadystatechange = function()
					{
						if (this.readyState == 4 && this.status == 200) 
						{
							req_done = true;
						}
					};
				}
			}
			
			function showHide(element_id, visible) 
			{
				if (document.getElementById(element_id)) //Если элемент с id-шником element_id существует 
				{ 
					var obj = document.getElementById(element_id); //Записываем ссылку на элемент в переменную obj
					if (visible) obj.style.display = "inline-block"; //Показываем элемент
					else obj.style.display = "none"; //Скрываем элемент
				}
				else alert("Элемент с id: " + element_id + " не найден!"); //Если элемент с id-шником element_id не найден, то выводим сообщение
			}   

			function process()
			{
				let url = '/jact';

				if (req_done)
				{
					req_done = false;
					var xh = new XMLHttpRequest();
					xh.open('GET', url, true);
					xh.send(null);
					xh.onreadystatechange = function()
					{
						if (this.readyState == 4 && this.status == 200) 
						{
						
							var res = JSON.parse(xh.responseText);

							document.getElementById('cur_br').value = res.ana_code;

							document.getElementById('rch1_value').value = res.ch1_value;
							document.getElementById('rch2_value').value = res.ch2_value;
							
							if ( res.ch1_state)  document.getElementById(  'ch1_on').value = 'On';
							if (!res.ch1_state)  document.getElementById(  'ch1_on').value = 'Off';
							if ( res.ch1_auto)   document.getElementById('ch1_auto').value = 'Auto';
							if (!res.ch1_auto)   document.getElementById('ch1_auto').value = 'Manual';
						
							if ( res.ch2_state)  document.getElementById(  'ch2_on').value = 'On';
							if (!res.ch2_state)  document.getElementById(  'ch2_on').value = 'Off';
							if ( res.ch2_auto)   document.getElementById('ch2_auto').value = 'Auto';
							if (!res.ch2_auto)   document.getElementById('ch2_auto').value = 'Manual';
	  
							var d1 = document.getElementById('lim_h').value;
							document.getElementById('rlim_h').value = d1;
	  
							d1 = document.getElementById('lim_l').value;
							document.getElementById('rlim_l').value = d1;
							
							req_done = true;
  					}
  				};
  			}
  		}
//------------------------------------------------------------------------------Common

      nIntervId = setInterval(process, 600);
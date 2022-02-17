let _from = 'none';

///-----------------------------------------------------------------------------WiFi.htm
			function loadVal_WiFi()
			{
				var xh = new XMLHttpRequest();
				xh.open("GET", "/jwifi", true);
				xh.send(null);
				xh.onreadystatechange =  function()
				{
					if (this.readyState == 4 && this.status == 200) 
					{
						var res = JSON.parse(xh.responseText);
						document.getElementById('aps' ).value   = res.as;
						document.getElementById('app' ).value   = res.ap;
						document.getElementById('ipap').value   = res.iap;
						document.getElementById('maap').value   = res.map;
						document.getElementById('stas1').value  = res.ss1;
						document.getElementById('stap1').value  = res.sp1;
						document.getElementById('sst1').checked = res.st1;
						document.getElementById('stas2').value  = res.ss2;
						document.getElementById('stap2').value  = res.sp2;
						document.getElementById('sst2').checked = res.st2;
						document.getElementById('wof').checked  = res.wof;

            
						var st = document.getElementById('sst1').checked;
						if (st)
						{
							document.getElementById('ipst1').value = res.ip1;
							document.getElementById('mast1').value = res.ma1;
							document.getElementById('gwst1').value = res.gw1;
							document.getElementById('dns1st1').value = res.d11;
							document.getElementById('dns2st1').value = res.d21;
						}
						st = document.getElementById('sst2').checked;
						if (st)
						{
							document.getElementById('ipst2').value = res.ip2;
							document.getElementById('mast2').value = res.ma2;
							document.getElementById('gwst2').value = res.gw2;
							document.getElementById('dns1st2').value = res.d12;
							document.getElementById('dns2st2').value = res.d22;
						}
						st_ip();
					}
				};
				_from = 'WiFi';
			}
			
			function st_ip()
			{
				var st = document.getElementById('sst1').checked;
				
				showHide('ipst1',   st);
				showHide('mast1',   st);
				showHide('gwst1',   st);
				showHide('dns1st1', st);
				showHide('dns2st1', st);

				st = document.getElementById('sst2').checked;

				showHide('ipst2',   st);
				showHide('mast2',   st);
				showHide('gwst2',   st);
				showHide('dns1st2', st);
				showHide('dns2st2', st);
			}

			function set_wifi()
			{
				var wof = 0, st1= 0, st2 = 0, ip, ma, gw, d1, d2, xh1, xh2, xh3; 

				var as = document.getElementById('aps').value;
				var ap = document.getElementById('app').value;
				var ss1 = document.getElementById('stas1').value;
				var sp1 = document.getElementById('stap1').value;
				var ss2 = document.getElementById('stas2').value;
				var sp2 = document.getElementById('stap2').value;

				if (document.getElementById('sst1').checked) st1 = 1;
				if (document.getElementById('sst2').checked) st2 = 1;

				var iap = document.getElementById('ipap').value;
				var map = document.getElementById('maap').value;

				if (document.getElementById('wof').checked) wof = 1;

				let urlc  = {
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

				let url = '/set_wifi?in=' + JSON.stringify(urlc);

				xh1 = new XMLHttpRequest();
				xh1.open('GET', url, true);
				xh1.send(null);
				xh1.onreadystatechange = function()
				{
  				if (this.readyState == 4 && this.status == 200)
  				{
    				if (st1 === 1)
    				{
    					ip = document.getElementById('ipst1').value;
    					ma = document.getElementById('mast1').value;
    					gw = document.getElementById('gwst1').value;
    					d1 = document.getElementById('dns1st1').value;
    					d2 = document.getElementById('dns2st1').value;
    
    				  urlc  = {
                          ip1: ip,
                          ma1: ma,
                          gw1: gw,
                          d11: d1,
                          d21: d2
                        };
    
    				  url = '/set_ip1?in=' + JSON.stringify(urlc);
    
    
    					xh2 = new XMLHttpRequest();
    					xh2.open('GET', url, true);
    					xh2.send(null);
    				}
    				
    				if (st2 === 1)
    				{
    					ip = document.getElementById('ipst2').value;
    					ma = document.getElementById('mast2').value;
    					gw = document.getElementById('gwst2').value;
    					d1 = document.getElementById('dns1st2').value;
    					d2 = document.getElementById('dns2st2').value;
    
    				  urlc  = {
                          ip2: ip,
                          ma2: ma,
                          gw2: gw,
                          d12: d1,
                          d22: d2
                        };
    
    				  url = '/set_ip2?in=' + JSON.stringify(urlc);
    
    					var xh3 = new XMLHttpRequest();
    					xh3.open('GET', url, true);
    					xh3.send(null);
      				xh3.onreadystatechange = function()
      				{
        				if (this.readyState == 4 && this.status == 200)
        				{
                  url = '/end_set_wifi';
          				var xh4 = new XMLHttpRequest();
          				xh4.open('GET', url, true);
          				xh4.send(null);
          				xh4.onreadystatechange = function()
          				{
          					if (this.readyState == 4 && this.status == 200) loadVal_WiFi();
          				};      
        				}
    				  };
			      }
			    }
				};
			}
//------------------------------------------------------------------------------Common
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

#!/bin/sh

WWW_ROOT=$(cd "$(dirname "$0")" && pwd)

if [ -z "$(which civetweb 2>/dev/null)" ]; then
    printf -- "ERROR: civetweb executable not found in PATH. "
    printf --   "Please add it and run this script again.\n" >&2
    printf -- "E.g.:\n" >&2
    printf -- "  PATH=\"%s:%s\" %s %s\n" \
      "./install/bin" \
      '${PATH}' \
      "${0}" \
      "${@}"
    exit 1
fi

civetweb -listening_ports 8000 \
         -document_root ${WWW_ROOT} \
         -cgi_interpreter /bin/sh
